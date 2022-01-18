#ifndef aFY_H
#define	aFY_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class aFX;
  
  class aFY : public std::vector< dtReal > {
    public:
      aFY( void );      
      aFY( const aFY& orig );
      aFY( const aFX& orig );
      aFY( dtInt const & ss, double const & ref );
      std::vector< dtReal > stdVector( void );
  };
}
#endif	/* aFY_H */

