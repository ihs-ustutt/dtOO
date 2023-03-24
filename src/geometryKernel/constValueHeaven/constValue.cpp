#include "constValue.h"
#include <limits>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include "sliderFloatParam.h"
#include "intParam.h"
#include "constrainedFloatParam.h"
#include <xmlHeaven/dtXmlParser.h>

namespace dtOO {
  constValue::constValue( constValue const & orig ) : labelHandling(orig) {
    _value = orig._value;
    _loadable = orig._loadable;
    _max = orig._max;
    _min = orig._min;
  }

  constValue::constValue( 
    std::string const & label, dtReal const & val
  ) : labelHandling(label) {
    _loadable = true;
    _value = val;
    _min = std::numeric_limits< dtReal >::min();
    _max = std::numeric_limits< dtReal >::max();
  }  
  
  constValue::~constValue() {
  }

  constValue * constValue::create(
    std::string const & type, 
    std::string const & label, 
    std::string const & valueStr,
    bool const loadable
  ) {
    constValue * aCV;
    if (type == "sliderFloatParam") {
      aCV 
      = 
      new sliderFloatParam(
        label,
        stringPrimitive::stringToFloat(valueStr) 
      );
    }
    else if (type == "intParam") {
      aCV 
      = 
      new intParam(
        label, 
        stringPrimitive::stringToInt(valueStr) 
      );
    }
    else if (type == "constrainedFloatParam") {
      aCV = new constrainedFloatParam( label, valueStr );
    }
    else dt__throwUnexpected(create());
    
    aCV->_loadable = loadable;
    return aCV;
  }

  void constValue::setRange(dtReal const min, dtReal const max) {
    _min = min;
    _max = max;
  }

  dtReal constValue::getValue(void) const {
    return _value;
  }
  
  std::string constValue::getValueStr(void) const {
    return stringPrimitive::floatToString( _value );
  }

  dtReal constValue::getValuePercent(void) const {
    return (_value - this->getMin()) / (this->getMax()-this->getMin());
  }
  
  dtReal constValue::operator()( void ) const {
    return this->getValue();
  }
  
  void constValue::setValue(dtReal const toSet) {
    if (toSet != _value) {
      dtXmlParser::reference().freeState();
      _value = toSet;
    }
  }

  dtReal constValue::getMax(void) const {
    return _max;
  }

  dtReal constValue::getMin(void) const {
    return _min;
  }
  
  bool constValue::loadable( void ) const {
    return this->_loadable;  
  }
  
  void constValue::resolveConstraint( 
    lvH_constValue const * const cVArr
  ) {
    
  }
  dt__C_addCloneForpVH(constValue);
}
