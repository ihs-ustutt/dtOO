#include "constValue.h"
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
  constValue::constValue() : labelHandling() {
  }

  constValue::constValue( constValue const & orig ) : labelHandling(orig) {
    _value = orig._value;
  }
  
  constValue::~constValue() {
  }

  void constValue::setRange(float const min, float const max) {
    dt__warning(setRange(), << "Call on constValue.");
  }

  float constValue::getValue(void) const {
    return _value;
  }

  void constValue::setValue(float const toSet) {
    _value = toSet;
  }

  float constValue::getMax(void) const {
    return 0.;
  }

  float constValue::getMin(void) const {
    return 0.;
  }
}