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
  
  std::vector< dtReal > aFX::stdVector( void ) {
    std::vector< dtReal > ret = std::vector< dtReal >( this->size(), 0. );
    dt__forFromToIndex(0, this->size() , ii ) ret[ ii ] = this->at(ii);
    return ret;
  }
}
