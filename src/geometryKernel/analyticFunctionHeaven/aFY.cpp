#include "aFY.h"

#include <logMe/logMe.h>
#include "aFX.h"

namespace dtOO {
  aFY::aFY( void ) : std::vector< float >() {

  }    
  aFY::aFY( const aFY& orig ) : std::vector< float >( orig ) {

  }
  
  aFY::aFY( const aFX& orig ) : std::vector< float >( orig.size(), 0. ) {
    dt__forAllIndex( orig, ii ) this->operator[]( ii ) = orig.at( ii );
  }
  
  aFY::aFY( int const & ss, double const & ref ) 
    : std::vector< float >(ss, static_cast< float >(ref) ) {

  }

  std::vector< float > aFY::stdVector( void ) {
    std::vector< float > ret = std::vector< float >( this->size(), 0. );
    dt__forFromToIndex(0, this->size() , ii ) ret[ ii ] = this->at(ii);
    return ret;
  }  
}
