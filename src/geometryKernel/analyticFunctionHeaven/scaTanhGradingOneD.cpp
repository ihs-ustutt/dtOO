#include "scaTanhGradingOneD.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  scaTanhGradingOneD::scaTanhGradingOneD() 
    : scaOneDPolyInterface() {
    setMinMax(0., 1.);
    _gg = 1.;
    _ggMin = 1.;
    _ggMax = 1.;
    _cc = std::vector< float >(4, 1.);
  }
  

  scaTanhGradingOneD::scaTanhGradingOneD(
    scaTanhGradingOneD const & orig
  ) : scaOneDPolyInterface(orig) {
    _gg = orig._gg;
    _ggMin = orig._ggMin;
    _ggMax = orig._ggMax;
    _cc = orig._cc;
  }

  scaTanhGradingOneD::scaTanhGradingOneD( 
    std::vector< float > cc, 
    float const & gg, float const & ggMin, float const & ggMax
  ) : scaOneDPolyInterface() {
    setMinMax(0., 1.);
    _gg = gg;
    _ggMin = ggMin;
    _ggMax = ggMax;
    _cc = cc;
  }

  scaTanhGradingOneD::~scaTanhGradingOneD() {

  }

  scaTanhGradingOneD * scaTanhGradingOneD::clone( void ) const {
    return new scaTanhGradingOneD( *this );
  }
  
	scaTanhGradingOneD * scaTanhGradingOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return 
      new analyticFunctionTransformed< scaTanhGradingOneD >(*this, dtT);
	}  
  
  scaTanhGradingOneD * scaTanhGradingOneD::create( void ) const {
    return new scaTanhGradingOneD();
  }

  float scaTanhGradingOneD::YFloat(float const & xx) const {
    return 
      _cc[0] 
      + 
      _cc[1] * tanh( _gg * (_cc[2] + _cc[3] * xx) )
      /
      tanh(_gg);
  }
  
  int scaTanhGradingOneD::nDOF( void ) const {
    return 1;
  }
  
  void scaTanhGradingOneD::setDOF( 
    std::vector< float > const value 
  ) {
    dt__throwIf( value.size()!=nDOF(), setDOF() );
    _gg = _ggMin +  value[0] * (_ggMax - _ggMin);
  }
}