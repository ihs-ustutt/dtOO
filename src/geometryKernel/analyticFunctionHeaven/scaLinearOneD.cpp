#include "scaLinearOneD.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/stringPrimitive.h>
#include "analyticFunctionTransformed.h"

namespace dtOO {
  scaLinearOneD::scaLinearOneD() : scaOneD() {
  }

  scaLinearOneD::scaLinearOneD(
    scaLinearOneD const & orig
  ) : scaOneD(orig) {
    _m = orig._m;
    _b = orig._b;
  }

  scaLinearOneD::scaLinearOneD(
    dtReal const & x0, dtReal const & x1, dtReal const & y0, dtReal const & y1
  ) : scaOneD() {
    dt__throwIf(x0>=x1, scaLinearOneD());
    
    _m = (y1 - y0) / (x1 - x0);
    _b = y0 - _m * x0;
    
    setMinMax(x0, x1);
  }

  scaLinearOneD::~scaLinearOneD() {

  }

  scaLinearOneD * scaLinearOneD::clone( void ) const {
    return new scaLinearOneD( *this );
  }
  
	scaLinearOneD * scaLinearOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< scaLinearOneD >(*this, dtT);
	}  
  
  scaLinearOneD * scaLinearOneD::create( void ) const {
    return new scaLinearOneD();
  }

  dtReal scaLinearOneD::YFloat(dtReal const & xx) const {
    return _m * xx + _b;
  }

  dtReal scaLinearOneD::invYFloat(dtReal const & yy) const {
		return (yy - _b) / _m;
  }  
}