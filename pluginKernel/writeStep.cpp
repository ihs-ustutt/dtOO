#include "writeStep.h"
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>

#include <logMe/logMe.h>
//#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <StepData_StepModel.hxx>
#include <STEPControl_Writer.hxx>
#include <Geom_Surface.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRep_CurveOnSurface.hxx>
#include <Precision.hxx>
#include <TopLoc_Location.hxx>
#include <ShapeBuild_Edge.hxx>

namespace dtOO {  
  writeStep::writeStep() { 
  }

  writeStep::~writeStep() {
  }

	void writeStep::init( 
		QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
		vectorHandling< dtPlugin * > const * const pL
	) {
	  dtPlugin::init(element, bC, cV, aF, aG, bV, pL);
		
		_fn = dtXmlParserBase::getAttributeStr("filename", element);
		std::vector< QDomElement > aGEl 
		= 
		dtXmlParserBase::getChildVector("analyticGeometry", element);
		for (int ii=0; ii<aGEl.size(); ii++) {
			_aG.push_back(
			  dtXmlParserBase::createAnalyticGeometry(&aGEl[ii], cV, aF, aG)
			);
		}
	}
		
  void writeStep::apply(void) {  
		STEPControl_Writer writer;
	  Handle(StepData_StepModel) model = writer.Model();
//		dt__FORALL(_aG, ii,
		for (int ii=0; ii<_aG.size(); ii++) {
			analyticSurface * aS = analyticSurface::DownCast(_aG[ii]);
			vec2dOneDInMap2dTo3d * v2d1d = vec2dOneDInMap2dTo3d::DownCast(_aG[ii]);
			if ( aS ) {
				dt__ptrAss(dtSurface const * dtS, aS->constPtrDtSurface());
				dt__ptrAss(dtOCCSurface const * dtOccS, dtOCCSurface::ConstDownCast(dtS));
				
		    writer.Transfer(
					BRepBuilderAPI_MakeFace(
						dtOccS->OCCRef().getOCC(), Precision::Confusion()
					).Shape(),
					STEPControl_StepModelType::STEPControl_AsIs								
				);
			}
			else if ( v2d1d ) {
				dt__ptrAss(
					analyticSurface const * aS, 
					analyticSurface::ConstDownCast(v2d1d->ptrToMap2dTo3d()) 
				);
				dt__ptrAss(dtSurface const * dtS, aS->ptrDtSurface());
				dt__ptrAss(dtOCCSurface const * dtOccS, dtOCCSurface::ConstDownCast(dtS));
				dt__ptrAss(
					vec2dCurve2dOneD const * v2dC1d, 
					vec2dCurve2dOneD::ConstDownCast(v2d1d->ptrToVec2dOneD()) 
				);
				dt__ptrAss(
				  dtOCCCurve2d const * dtOccC2d, 
					dtOCCCurve2d::ConstDownCast(v2dC1d->ptrDtCurve2d())
				);
			
        TopoDS_Edge edge;
				ShapeBuild_Edge buildEdge;
				buildEdge.MakeEdge(
					edge, 
					dtOccC2d->OCCRef().getOCC(), 
					BRepBuilderAPI_MakeFace(
						dtOccS->OCCRef().getOCC(), Precision::Confusion()
					).Face()
				);
//				TopoDS_Shape shape(edge);
		    writer.Transfer( edge, STEPControl_StepModelType::STEPControl_AsIs	);
			}			
		}
		writer.Write(_fn.c_str());
  }
}


