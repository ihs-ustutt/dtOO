#include "bSplineSurface_skinConstructOCC.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCBSplineSurface.h>

#include <Precision.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <GeomFill_SectionGenerator.hxx>
#include <GeomFill_Line.hxx>
#include <GeomFill_AppSurf.hxx>
#include <BSplCLib.hxx>
#include <Geom_BSplineSurface.hxx>


namespace dtOO {
  bSplineSurface_skinConstructOCC::bSplineSurface_skinConstructOCC() {
		
	}
	
	/**
	 * @todo ptrVectorHandling as input
	 */
  bSplineSurface_skinConstructOCC::bSplineSurface_skinConstructOCC(
	  vectorHandling< dtCurve const * > const &  cL
	) {
    _dtS.reset(
      bSplineSurface_skinConstructOCC(cL, 1, BSplCLib::MaxDegree(), 0).result()
    );    
	}
  
  bSplineSurface_skinConstructOCC::bSplineSurface_skinConstructOCC(
	  vectorHandling< dtCurve const * > const &  cL, 
    int const & minDeg, int const & maxDeg, int const & nIter
	) {
		GeomFill_SectionGenerator aSecGen;
		dt__forAllIndex(cL, ii) {
		  dt__ptrAss( dtOCCCurve * bC, dtOCCCurve::DownCast(cL[ii]->clone()) );
      aSecGen.AddCurve( bC->OCCRef().getOCC() );
      delete bC;			
		}
		aSecGen.Perform( Precision::Parametric(Precision::Confusion()) );
		
		Handle(GeomFill_Line) aLine = new GeomFill_Line(cL.size() );
		const Standard_Integer aMinDeg = minDeg;
		const Standard_Integer aMaxDeg = maxDeg;
		const Standard_Integer aNbIt = nIter;
		Standard_Real aTol3d = Precision::Confusion();
		Standard_Real aTol2d = Precision::Parametric(aTol3d);
		
		GeomFill_AppSurf anAlgo;
		dt__tryOcc(
			anAlgo.Init(aMinDeg, aMaxDeg, aTol3d, aTol2d, aNbIt);
	  ,
		  << ""
		);
		dt__tryOcc(
			anAlgo.Perform(aLine, aSecGen);
	  ,
		  << ""
		);		
		
		Handle(Geom_Surface) aRes;
		dt__tryOcc(
      aRes = new Geom_BSplineSurface(
        anAlgo.SurfPoles(), anAlgo.SurfWeights(),
        anAlgo.SurfUKnots(), anAlgo.SurfVKnots(),
        anAlgo.SurfUMults(), anAlgo.SurfVMults(),
        anAlgo.UDegree(), anAlgo.VDegree()
      );
    ,
      << ""
    );
		
		dtOCCSurfaceBase base;
		base.setOCC(aRes);
		
		_dtS.reset( new dtOCCBSplineSurface(base) );
	}  
	
  bSplineSurface_skinConstructOCC::~bSplineSurface_skinConstructOCC() {
		
	}
	
  dtSurface * bSplineSurface_skinConstructOCC::result( void ) {
		return _dtS->clone();
	}
}