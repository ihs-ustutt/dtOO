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

#include "aFY.h"

#include <logMe/logMe.h>
#include "aFX.h"

namespace dtOO {
  aFY::aFY( void ) : std::vector< dtReal >() {

  }    
  aFY::aFY( const aFY& orig ) : std::vector< dtReal >( orig ) {

  }
  
  aFY::aFY( const aFX& orig ) : std::vector< dtReal >( orig.size(), 0. ) {
    dt__forAllIndex( orig, ii ) this->operator[]( ii ) = orig.at( ii );
  }
  
  aFY::aFY( dtInt const & ss, double const & ref ) 
    : std::vector< dtReal >(ss, static_cast< dtReal >(ref) ) {

  }

  std::vector< dtReal > aFY::stdVector( void ) {
    std::vector< dtReal > ret = std::vector< dtReal >( this->size(), 0. );
    dt__forFromToIndex(0, this->size() , ii ) ret[ ii ] = this->at(ii);
    return ret;
  }  
}
