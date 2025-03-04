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

#include "aFX.h"

#include <logMe/logMe.h>
#include "aFY.h"

namespace dtOO {
  aFX::aFX( void ) : std::vector< dtReal >() {

  }  
  aFX::aFX( const aFX& orig ) : std::vector< dtReal >( orig ) {

  }
  aFX::aFX( const aFY& orig ) : std::vector< dtReal >( orig.size(), 0. ) {
    dt__forAllIndex( orig, ii ) this->operator[]( ii ) = orig.at( ii );
  }           
  aFX::aFX( dtInt const & ss, double const & ref ) 
    : std::vector< dtReal >(ss, static_cast< dtReal >(ref) ) {
  }
  
  aFX::aFX( std::vector< dtReal > const & orig ) : std::vector< dtReal >( orig ) {
    
  }
  
  std::vector< dtReal > aFX::toStdVector( void ) {
    std::vector< dtReal > ret = std::vector< dtReal >( this->size(), 0. );
    dt__forFromToIndex(0, this->size() , ii ) ret[ ii ] = this->at(ii);
    return ret;
  }
}
