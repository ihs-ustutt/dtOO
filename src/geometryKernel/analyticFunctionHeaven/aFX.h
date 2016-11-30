#ifndef aFX_H
#define	aFX_H

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class aFY;
  
  class aFX : public std::vector< float > {
    public:
      aFX( void );
      aFX( const aFX& orig );
      aFX( const aFY& orig );
      aFX( std::vector< float > const & orig );
      aFX( int const & ss, double const & ref );
      std::vector< float > stdVector( void );
  };
}
#endif	/* aFX_H */

