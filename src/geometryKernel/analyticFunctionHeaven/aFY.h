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
