/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "floatValue.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  floatValue::floatValue( 
    std::string const & label, dtReal const & value 
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
  
  dtReal floatValue::operator()( void ) {
    return _value;
  }
}
