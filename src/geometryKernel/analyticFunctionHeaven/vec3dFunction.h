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

#ifndef VEC3DFUNCTION_H
#define	VEC3DFUNCTION_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "analyticFunction.h"

namespace dtOO {
  class vec3dFunction : public analyticFunction {
    public:
      dt__class(vec3dFunction, analyticFunction);     
      vec3dFunction();
      vec3dFunction(const vec3dFunction& orig);
      virtual vec3dFunction * clone( void ) const = 0;
      virtual ~vec3dFunction();
      virtual dtInt yDim( void ) const;    
      dtPoint3 YdtPoint3(aFX const & xx) const;
      virtual bool closed( dtInt const & dir ) const = 0;
  };
  dt__H_addCloneForpVH(vec3dFunction);
}
#endif	/* VEC3DFUNCTION_H */
