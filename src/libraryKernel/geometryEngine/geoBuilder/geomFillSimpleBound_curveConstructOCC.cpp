#include "geomFillSimpleBound_curveConstructOCC.h"

#include <logMe/logMe.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCGeomFillBoundaryBase.h>
#include <geometryEngine/dtOCCGeomFillSimpleBound.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Geom_Curve.hxx>
#include <GeomFill_SimpleBound.hxx>
#include <GeomAdaptor_HCurve.hxx>

namespace dtOO {
	geomFillSimpleBound_curveConstructOCC
    ::geomFillSimpleBound_curveConstructOCC( 
      dtCurve const * const dtC
  ) {
    dt__ptrAss(
		  dtOCCCurve const * occC, 
			dtOCCCurve::ConstDownCast(dtC)
		);
    Handle(Geom_Curve) myParamCurve
    = 
    Handle(Geom_Curve)::DownCast( occC->OCCRef().getOCC()->Copy() );

    Handle(GeomAdaptor_HCurve) ParamCurve 
    = 
    new GeomAdaptor_HCurve(myParamCurve);

    Handle(GeomFill_SimpleBound) myBoundary 
    = 
    new GeomFill_SimpleBound(
      ParamCurve, Precision::Confusion(), Precision::Angular()
    );

    dt__tryOcc(
      _dtC.reset( 
        new dtOCCGeomFillSimpleBound(
          dtOCCCurveBase( 
            Handle(Geom_Curve)::DownCast(occC->OCCRef().getOCC()->Copy()) 
          ), 
          dtOCCGeomFillBoundaryBase( myBoundary )
        ) 
      );
      , << ""
    );
	}
	
	geomFillSimpleBound_curveConstructOCC
    ::~geomFillSimpleBound_curveConstructOCC() {
	}
	
	dtCurve * geomFillSimpleBound_curveConstructOCC::result( void ) {
		return _dtC->clone();
	}
}