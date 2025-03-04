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

#ifndef vectorFunctionDist_H
#define	vectorFunctionDist_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include "floatAtt.h"

namespace dtOO { 
  class analyticFunction;

  class vectorFunctionDist : public floatAtt {
    public:  
      dt__classOnlyName(vectorFunctionDist);
      vectorFunctionDist(
        std::vector< dtReal > const & vec,
        analyticFunction const * const aF
      );
      virtual ~vectorFunctionDist();
      vectorFunctionDist( vectorFunctionDist const & orig );
      virtual vectorFunctionDist * clone( void ) const;
      virtual dtReal operator()( std::vector< dtReal > const & xx ) const;
      //
      // get properties
      //
      virtual dtInt const & dimension() const;
    private:
      std::vector< dtReal > const _vec;
      dtInt const _dimension;
      analyticFunction const * const _aF;
  };
  dt__H_addCloneForpVH(vectorFunctionDist);
}
#endif	/* vectorFunctionDist_H */
