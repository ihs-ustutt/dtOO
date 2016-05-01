#include "floatValue.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  floatValue::floatValue( 
    std::string const & label, float const & value 
  ) : resultValue( label ) {
    _value = value;
  }
  
  floatValue::floatValue( floatValue const & orig ) : resultValue(orig) {
    _value = orig._value;
  }
  
  floatValue::~floatValue() {
  }

  floatValue * floatValue::clone( void ) const {
    return new floatValue( *this );
  }
  
  float floatValue::operator()( void ) {
    return _value;
  }
}