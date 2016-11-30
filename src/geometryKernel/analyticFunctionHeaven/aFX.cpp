#include "aFX.h"

#include <logMe/logMe.h>
#include "aFY.h"

namespace dtOO {
  aFX::aFX( void ) : std::vector< float >() {

  }  
  aFX::aFX( const aFX& orig ) : std::vector< float >( orig ) {

  }
  aFX::aFX( const aFY& orig ) : std::vector< float >( orig.size(), 0. ) {
    dt__forAllIndex( orig, ii ) this->operator[]( ii ) = orig.at( ii );
  }           
  aFX::aFX( int const & ss, double const & ref ) 
    : std::vector< float >(ss, static_cast< float >(ref) ) {
  }
  
  aFX::aFX( std::vector< float > const & orig ) : std::vector< float >( orig ) {
    
  }
  
  std::vector< float > aFX::stdVector( void ) {
    std::vector< float > ret = std::vector< float >( this->size(), 0. );
    dt__forFromToIndex(0, this->size() , ii ) ret[ ii ] = this->at(ii);
    return ret;
  }
}
