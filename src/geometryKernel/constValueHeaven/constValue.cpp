#include "constValue.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include "sliderFloatParam.h"
#include "intParam.h"
#include "constrainedFloatParam.h"
#include <xmlHeaven/dtXmlParser.h>

namespace dtOO {
  constValue::constValue() : labelHandling() {
    _loadable = true;
    _value = 0.;
  }

  constValue::constValue( constValue const & orig ) : labelHandling(orig) {
    _value = orig._value;
    _loadable = orig._loadable;
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
    if (type == "sliderFloatParam") aCV = new sliderFloatParam();  
    else if (type == "intParam") aCV = new intParam();
    else if (type == "constrainedFloatParam") {
      aCV = new constrainedFloatParam( valueStr );
    }
    else dt__throwUnexpected(create());
    
    aCV->setLabel(label);
    aCV->_loadable = loadable;
    return aCV;
  }

  void constValue::setRange(dtReal const min, dtReal const max) {
    dt__warning(setRange(), << "Call on constValue.");
  }

  dtReal constValue::getValue(void) const {
    return _value;
  }

  dtReal constValue::getValuePercent(void) const {
    return (_value - this->getMin()) / (this->getMax()-this->getMin());
  }
  
  void constValue::setValue(dtReal const toSet) {
    if (toSet != _value) {
      dtXmlParser::reference().freeState();
      _value = toSet;
    }
  }

  dtReal constValue::getMax(void) const {
    return 0.;
  }

  dtReal constValue::getMin(void) const {
    return 0.;
  }
  
  bool constValue::loadable( void ) const {
    return this->_loadable;  
  }
  
  void constValue::resolveConstraint( 
    cVPtrVec const * const cVArr
  ) {
    
  }
}