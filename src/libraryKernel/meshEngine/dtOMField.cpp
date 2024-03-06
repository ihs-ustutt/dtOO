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

#include "dtOMField.h"

namespace dtOO {
  dtOMField::dtOMField( std::string const & label, dtOMMesh const & om ) 
    : labelHandling(label), _om(om) {
  }
  
  dtOMField::~dtOMField() {
  }
  
  dtOMMesh const & dtOMField::refMesh( void ) const {
    return _om;
  }
}
