#include "scaSingleTanhGradingOneD.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  scaSingleTanhGradingOneD::scaSingleTanhGradingOneD() 
    : scaOneDPolyInterface() {
    setMinMax(0., 1.);
    _gg = 1.;
    _ggMin = 1.;
    _ggMax = 1.;
  }
  

  scaSingleTanhGradingOneD::scaSingleTanhGradingOneD(
    scaSingleTanhGradingOneD const & orig
  ) : scaOneDPolyInterface(orig) {
    _gg = orig._gg;
    _ggMin = orig._ggMin;
    _ggMax = orig._ggMax;
  }

  scaSingleTanhGradingOneD::scaSingleTanhGradingOneD( 
    float const & gg, float const & ggMin, float const & ggMax
  ) 
    : scaOneDPolyInterface() {
    setMinMax(0., 1.);
    _gg = gg;
    _ggMin = ggMin;
    _ggMax = ggMax;
  }

  scaSingleTanhGradingOneD::~scaSingleTanhGradingOneD() {

  }

  scaSingleTanhGradingOneD * scaSingleTanhGradingOneD::clone( void ) const {
    return new scaSingleTanhGradingOneD( *this );
  }
  
	scaSingleTanhGradingOneD * scaSingleTanhGradingOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return 
      new analyticFunctionTransformed< scaSingleTanhGradingOneD >(*this, dtT);
	}  
  
  scaSingleTanhGradingOneD * scaSingleTanhGradingOneD::create( void ) const {
    return new scaSingleTanhGradingOneD();
  }

  float scaSingleTanhGradingOneD::YFloat(float const & xx) const {
    return 
      1. 
      - 
      tanh( _gg * (1.-xx) )
      /
      tanh(_gg);
  }
  
  int scaSingleTanhGradingOneD::nDOF( void ) const {
    return 1;
  }
  
  void scaSingleTanhGradingOneD::setDOF( 
    std::vector< float > const value 
  ) {
    dt__throwIf( value.size()!=nDOF(), setDOF() );
    _gg = _ggMin +  value[0] * (_ggMax - _ggMin);
  }
}