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

#ifndef VEC3DBOXTHREED_H
#define	VEC3DBOXTHREED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dThreeD.h"
#include "vec3dTwoD.h"
#include "analyticFunctionCompound.h"
#include <interfaceHeaven/ptrVectorHandling.h>

namespace dtOO {
  class vec3dTwoD;
  class dtTransformer;  
  
  class vec3dBoxThreeD : public vec3dThreeD {
  public:
    dt__class(vec3dBoxThreeD, analyticFunction);     
    vec3dBoxThreeD();
    vec3dBoxThreeD(const vec3dBoxThreeD& orig);
    vec3dBoxThreeD(dtPoint3 const & p0, dtPoint3 const & p1);
    virtual vec3dBoxThreeD * clone( void ) const;
    virtual vec3dBoxThreeD * cloneTransformed(
      dtTransformer const * const dtT 
    ) const;      
    virtual vec3dBoxThreeD * create( void ) const;    
    virtual ~vec3dBoxThreeD();
    virtual aFY Y( aFX const & xx) const;
    virtual bool closed( dtInt const & dir ) const;
  private:
    dtPoint3 _p0;   
    dtPoint3 _p1;
  };
  typedef analyticFunctionCompound<vec3dBoxThreeD> vec3dBoxThreeDCompound;
}
#endif	/* VEC3DBOXTHREED_H */
