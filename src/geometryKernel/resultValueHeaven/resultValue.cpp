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
