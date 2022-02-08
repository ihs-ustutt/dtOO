#include "dtOCCGeomFillSimpleBound.h"

#include <logMe/logMe.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include "dtOCCCurveBase.h"
#include "dtOCCGeomFillBoundaryBase.h"

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_TypeDef.hxx>
#include <Adaptor3d_Curve.hxx>
#include <GeomFill_SimpleBound.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {				
	dtOCCGeomFillSimpleBound::dtOCCGeomFillSimpleBound(
  ) : dtOCCGeomFillBoundary() {
		_ptr = NULL;
	}

	dtOCCGeomFillSimpleBound::dtOCCGeomFillSimpleBound( 
    dtOCCCurveBase const & cBase, dtOCCGeomFillBoundaryBase const & bBase
  ) : dtOCCGeomFillBoundary(cBase, bBase) {
		dt__mustCast(
      OCCBoundaryRef().getOCC().get(), GeomFill_SimpleBound const, _ptr
    );
	}
	
	dtOCCGeomFillSimpleBound::~dtOCCGeomFillSimpleBound() {
	}

  dtCurve * dtOCCGeomFillSimpleBound::clone( void ) const {
		return new dtOCCGeomFillSimpleBound( OCCRef(), OCCBoundaryRef() );
	}
  
  dtReal dtOCCGeomFillSimpleBound::minPara( dtInt const & dir ) const {
    dt__throwIf(dir!=0, minPara());
    Standard_Real U0;
    Standard_Real U1;
    dt__tryOcc(
      _ptr->Bounds(U0, U1);
      ,
      << " U = [ " << U0 << ", " << U1 << " ]"
    );
    return floatHandling::nextIfSmaller(U0);
  }
  
  dtReal dtOCCGeomFillSimpleBound::maxPara( dtInt const & dir ) const {
    dt__throwIf(dir!=0, maxPara());
    Standard_Real U0;
    Standard_Real U1;
    dt__tryOcc(
      _ptr->Bounds(U0, U1);
      ,
      << " U = [ " << U0 << ", " << U1 << " ]"
    );
    return floatHandling::nextIfSmaller(U1);    
  }
        
  dtPoint3 dtOCCGeomFillSimpleBound::point( dtReal const uu ) const {
		Standard_Real uR = static_cast<Standard_Real>(uu);
		gp_Pnt pp;
		dt__tryOcc(
		  pp = _ptr->Value(uR);
		,
		  << dt__eval(uu)
		);
		
		return dtPoint3(
			static_cast<dtReal>(pp.Coord(1)), 
			static_cast<dtReal>(pp.Coord(2)), 
			static_cast<dtReal>(pp.Coord(3))
		);    
  }
}
