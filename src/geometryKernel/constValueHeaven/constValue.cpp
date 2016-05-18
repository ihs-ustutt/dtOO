#include "constValue.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include "sliderFloatParam.h"
#include "intParam.h"
#include <xmlHeaven/dtXmlParser.h>

namespace dtOO {
  constValue::constValue() : labelHandling() {
  }

  constValue::constValue( constValue const & orig ) : labelHandling(orig) {
    _value = orig._value;
  }
  
  constValue::~constValue() {
  }

  constValue * constValue::create(
    std::string const & type, std::string const & label
  ) {
    constValue * aCV;
    if (type == "sliderFloatParam") aCV = new sliderFloatParam();  
    else if (type == "intParam") aCV = new intParam();
    else dt__throwUnexpected(create());
    
    aCV->setLabel(label);
    return aCV;
  }

  void constValue::setRange(float const min, float const max) {
    dt__warning(setRange(), << "Call on constValue.");
  }

  float constValue::getValue(void) const {
    return _value;
  }

  void constValue::setValue(float const toSet) {
    if (toSet != _value) {
      dtXmlParser::reference().freeState();
      _value = toSet;
    }
  }

  float constValue::getMax(void) const {
    return 0.;
  }

  float constValue::getMin(void) const {
    return 0.;
  }
}