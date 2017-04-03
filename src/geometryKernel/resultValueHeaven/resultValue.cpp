#include "resultValue.h"

#include <logMe/logMe.h>

namespace dtOO {
  resultValue::resultValue( std::string const & label ) : labelHandling(label) {
  }

  resultValue::resultValue( resultValue const & orig ) : labelHandling(orig) {
  }
  
  resultValue::~resultValue() {
  }
}