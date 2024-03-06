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

#ifndef VEC2DFUNCTION_H
#define	VEC2DFUNCTION_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunction.h"

namespace dtOO {
  class vec2dFunction : public analyticFunction {
    public:
      dt__class(vec2dFunction, analyticFunction);     
      vec2dFunction();
      vec2dFunction(const vec2dFunction& orig);
      virtual vec2dFunction * clone( void ) const = 0;
      virtual ~vec2dFunction();
      virtual dtInt yDim( void ) const;    
      dtPoint2 YdtPoint2(aFX const & xx) const;
      virtual bool closed( dtInt const & dir ) const = 0;    
  };
  dt__H_addCloneForpVH(vec2dFunction);
}
#endif	/* VEC2DFUNCTION_H */
