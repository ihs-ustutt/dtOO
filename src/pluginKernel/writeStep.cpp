#include "writeStep.h"

#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h>

#include <logMe/logMe.h>
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
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Precision.hxx>
#include <ShapeBuild_Edge.hxx>
#include "dtPluginFactory.h"

namespace dtOO {  
  bool writeStep::_registrated 
  =
  dtPluginFactory::registrate( dt__tmpPtr(writeStep, new writeStep()) );
    
  writeStep::writeStep() { 
    _translator = 0;
  }

  writeStep::~writeStep() {
  }

  void writeStep::init( 
    ::QDomElement const & element,
    baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtCase const * const dC,    
    lvH_dtPlugin const * const pL
  ) {
    dtPlugin::init(element, bC, cV, aF, aG, bV, dC, pL);

    _fn = dtXmlParserBase::getAttributeStr("filename", element);
    std::vector< ::QDomElement > aGEl 
    = 
    dtXmlParserBase::getChildVector("analyticGeometry", element);
    for (int ii=0; ii<aGEl.size(); ii++) {
      _aG.push_back(
        dtXmlParserBase::createAnalyticGeometry(&aGEl[ii], bC, cV, aF, aG)
      );
    }
    if ( dtXmlParserBase::hasAttribute("translator", element) ) {
      _translator = dtXmlParserBase::getAttributeInt("translator", element);
    }
  }
		
  void writeStep::apply(void) {
    STEPControl_Writer writer;
    Handle(StepData_StepModel) model = writer.Model();
//		dt__FORALL(_aG, ii,
    for (int ii=0; ii<_aG.size(); ii++) {
      analyticCurve * aC = analyticCurve::DownCast(_aG[ii]);
      analyticSurface * aS = analyticSurface::DownCast(_aG[ii]);
      vec2dOneDInMap2dTo3d * v2d1d = vec2dOneDInMap2dTo3d::DownCast(_aG[ii]);
      if ( aC ) {
        dt__ptrAss(dtCurve const * dtC, aC->ptrConstDtCurve());
        dt__ptrAss(dtOCCCurve const * dtOccC, dtOCCCurve::ConstDownCast(dtC));

        writer.Transfer(
          BRepBuilderAPI_MakeEdge(dtOccC->OCCRef().getOCC()).Shape(),
          static_cast< STEPControl_StepModelType >(_translator)
        );
      }      
      else if ( aS ) {
        dt__ptrAss(dtSurface const * dtS, aS->ptrConstDtSurface());
        dt__ptrAss(dtOCCSurface const * dtOccS, dtOCCSurface::ConstDownCast(dtS));

        writer.Transfer(
          BRepBuilderAPI_MakeFace(
            dtOccS->OCCRef().getOCC(), Precision::Confusion()
          ).Shape(),
          static_cast< STEPControl_StepModelType >(_translator)
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
        writer.Transfer( 
          edge, static_cast< STEPControl_StepModelType >(_translator)
        );
      }			
    }
    writer.Write(_fn.c_str());
  }
}


