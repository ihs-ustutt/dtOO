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
  
  aFY::aFY( int const & ss, double const & ref ) 
    : std::vector< dtReal >(ss, static_cast< dtReal >(ref) ) {

  }

  std::vector< dtReal > aFY::stdVector( void ) {
    std::vector< dtReal > ret = std::vector< dtReal >( this->size(), 0. );
    dt__forFromToIndex(0, this->size() , ii ) ret[ ii ] = this->at(ii);
    return ret;
  }  
}
