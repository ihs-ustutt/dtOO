#include "dtOCCCompositeSurface.h"

#include <logMe/logMe.h>
#include "dtOCCSurfaceBase.h"
#include "dtOCCCurveBase.h"
#include "dtOCCBSplineCurve.h"
#include "dtOCCBezierCurve.h"
#include <logMe/dtMacros.h>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <ShapeExtend_CompositeSurface.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_Line.hxx>

#include "dtCurve.h"
#include "geoBuilder/geomCurve_baseConstructOCC.h"
#include "geoBuilder/bSplineCurve_curveConnectConstructOCC.h"

namespace dtOO {	
	dtOCCCompositeSurface::dtOCCCompositeSurface() : dtOCCSurface() {
		_ptr = NULL;
	}

	dtOCCCompositeSurface::dtOCCCompositeSurface(const dtOCCSurfaceBase& orig) 
		: dtOCCSurface(orig) {
		dt__mustCast(OCCRef().getOCC().Access(), ShapeExtend_CompositeSurface const, _ptr);
	}

	dtOCCCompositeSurface::~dtOCCCompositeSurface() {

	}

	dtSurface * dtOCCCompositeSurface::clone( void ) const {
		return new dtOCCCompositeSurface( OCCRef() );
	}

	dtCurve * dtOCCCompositeSurface::segmentConstU( 
    float const uu, float const vvMin, float const vvMax
  ) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
    Standard_Real v0R = static_cast<Standard_Real>(vvMin);
    Standard_Real v1R = static_cast<Standard_Real>(vvMax);
          
    if (_ptr->Patch(uR, v0R).Access() == _ptr->Patch(uR, v1R).Access() ) {
      Handle(Geom_Surface) ss = _ptr->Patch(uR, v0R);
      Handle(Geom_Curve) cc
      =
      ss->UIso(
        _ptr->UGlobalToLocal( 
          _ptr->LocateUParameter(uR), _ptr->LocateVParameter(v0R), uR 
        )
      );

      dtOCCCurveBase base;
      base.setOCC(cc);      
      
      return geomCurve_baseConstructOCC( base ).result();        
    }
    else {
      Standard_Integer I = _ptr->LocateUParameter(uR);
      Standard_Integer J0 = _ptr->LocateVParameter(v0R);
      Standard_Integer J1 = _ptr->LocateVParameter(v1R);
      
      vectorHandling< dtCurve const * > ccV;
      dt__forFromToIndex(J0, J1, jj) {
        Handle(Geom_Surface) ss = _ptr->Patch(I, jj);
        Handle(Geom_Curve) cLoc
        =
        ss->UIso( 
          _ptr->UGlobalToLocal( 
            _ptr->LocateVParameter(uR), _ptr->LocateUParameter(uR), uR 
          )          
        );
        dtOCCCurveBase baseLoc;
        baseLoc.setOCC( cLoc );
        ccV.push_back( geomCurve_baseConstructOCC(baseLoc).result() );
      }
      dtCurve * cc = bSplineCurve_curveConnectConstructOCC(ccV).result();
      ccV.destroy();
      return cc;
    }
	}

	dtCurve * dtOCCCompositeSurface::segmentConstV( 
    float const vv, float const uuMin, float const uuMax
  ) const {
		Standard_Real vR = static_cast<Standard_Real>(vv);
    Standard_Real u0R = static_cast<Standard_Real>(uuMin);
    Standard_Real u1R = static_cast<Standard_Real>(uuMax);
    
    
    if ( _ptr->Patch(u0R, vR).Access() == _ptr->Patch(u1R, vR).Access() ) {
      Handle(Geom_Surface) ss = _ptr->Patch(u0R, vR);
      Handle(Geom_Curve) cc
      =
      ss->VIso(
        _ptr->VGlobalToLocal( 
          _ptr->LocateUParameter(u0R), _ptr->LocateVParameter(vR), vR 
        )
      );
      dtOCCCurveBase base;      
      base.setOCC(cc);      
      
      return geomCurve_baseConstructOCC( base ).result();      
    }
    else {
      Standard_Integer I0 = _ptr->LocateUParameter(u0R);
      Standard_Integer I1 = _ptr->LocateUParameter(u1R);
      Standard_Integer J = _ptr->LocateVParameter(vR);
      
      vectorHandling< dtCurve const * > ccV;
      dt__forFromToIndex(I0, I1, ii) {
        Handle(Geom_Surface) ss = _ptr->Patch(ii, J);
        Handle(Geom_Curve) cLoc
        =
        ss->VIso( 
          _ptr->VGlobalToLocal( 
            _ptr->LocateUParameter(u0R), _ptr->LocateVParameter(vR), vR 
          )          
        );
        dtOCCCurveBase baseLoc;
        baseLoc.setOCC( cLoc );
        ccV.push_back( geomCurve_baseConstructOCC(baseLoc).result() );
      }
      dtCurve * cc = bSplineCurve_curveConnectConstructOCC(ccV).result();
      ccV.destroy();
      return cc;
    }
	}		
}
