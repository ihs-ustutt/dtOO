#ifndef aFX_H
#define	aFX_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class aFY;
  
  class aFX : public std::vector< dtReal > {
    public:
      aFX( void );
      aFX( const aFX& orig );
      aFX( const aFY& orig );
      aFX( std::vector< dtReal > const & orig );
      aFX( dtInt const & ss, double const & ref );
      std::vector< dtReal > stdVector( void );
  };
}
#endif	/* aFX_H */

