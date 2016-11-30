#ifndef aFY_H
#define	aFY_H

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class aFX;
  
  class aFY : public std::vector< float > {
    public:
      aFY( void );      
      aFY( const aFY& orig );
      aFY( const aFX& orig );
      aFY( int const & ss, double const & ref );
      std::vector< float > stdVector( void );
  };
}
#endif	/* aFY_H */

