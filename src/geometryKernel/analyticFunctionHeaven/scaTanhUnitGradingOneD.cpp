#include "scaTanhUnitGradingOneD.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
  scaTanhUnitGradingOneD::scaTanhUnitGradingOneD() 
    : scaOneDPolyInterface() {
    setMinMax(0., 1.);
    _gg = 1.;
    _ggMin = 1.;
    _ggMax = 1.;
    _cc = std::vector< dtReal >(2, 1.);
  }
  

  scaTanhUnitGradingOneD::scaTanhUnitGradingOneD(
    scaTanhUnitGradingOneD const & orig
  ) : scaOneDPolyInterface(orig) {
    _gg = orig._gg;
    _ggMin = orig._ggMin;
    _ggMax = orig._ggMax;
    _cc = orig._cc;
  }

  scaTanhUnitGradingOneD::scaTanhUnitGradingOneD( 
    std::vector< dtReal > cc, 
    dtReal const & gg, dtReal const & ggMin, dtReal const & ggMax
  ) : scaOneDPolyInterface() {
    setMinMax(0., 1.);
    _gg = gg;
    _ggMin = ggMin;
    _ggMax = ggMax;
    _cc = cc;
  }

  scaTanhUnitGradingOneD::~scaTanhUnitGradingOneD() {

  }

  scaTanhUnitGradingOneD * scaTanhUnitGradingOneD::clone( void ) const {
    return new scaTanhUnitGradingOneD( *this );
  }
  
	scaTanhUnitGradingOneD * scaTanhUnitGradingOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return 
      new analyticFunctionTransformed< scaTanhUnitGradingOneD >(*this, dtT);
	}  
  
  scaTanhUnitGradingOneD * scaTanhUnitGradingOneD::create( void ) const {
    return new scaTanhUnitGradingOneD();
  }

  dtReal scaTanhUnitGradingOneD::YFloat(dtReal const & xx) const {
    return 
      (
        -tanh(_gg*_cc[0])/(tanh(_gg*(_cc[0]+_cc[1]))-tanh(_gg*_cc[0]))
      )
      + 
      (
        tanh(_gg)/(tanh(_gg*(_cc[0]+_cc[1]))-tanh(_gg*_cc[0]))
      ) 
      * 
      tanh( _gg * (_cc[0] + _cc[1] * xx) )
      /
      tanh(_gg);
  }
  
  int scaTanhUnitGradingOneD::nDOF( void ) const {
    return 1;
  }
  
  void scaTanhUnitGradingOneD::setDOF( 
    std::vector< dtReal > const value 
  ) {
    dt__throwIf( value.size()!=nDOF(), setDOF() );
    _gg = _ggMin +  value[0] * (_ggMax - _ggMin);
  }
}