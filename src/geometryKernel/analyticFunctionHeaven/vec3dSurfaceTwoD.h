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

#ifndef VEC3DSURFACETWOD_H
#define	VEC3DSURFACETWOD_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dTwoD.h"
#include <interfaceHeaven/ptrHandling.h>
#include "analyticFunctionCompound.h"

namespace dtOO {
  class dtSurface;
  class dtTransformer;
  
  class vec3dSurfaceTwoD : public vec3dTwoD {
    public:
      dt__class(vec3dSurfaceTwoD, analyticFunction);     
      vec3dSurfaceTwoD();
      vec3dSurfaceTwoD(const vec3dSurfaceTwoD& orig);
      vec3dSurfaceTwoD(dtSurface const * const dtS);
      virtual vec3dSurfaceTwoD * clone( void ) const;
      virtual vec3dSurfaceTwoD * cloneTransformed(
        dtTransformer const * const dtT 
      ) const;    
      virtual vec3dSurfaceTwoD * create( void ) const;    
      virtual ~vec3dSurfaceTwoD();
      virtual aFY Y( aFX const & xx) const;
      dtSurface * ptrDtSurface( void ) const;
      dtSurface const * constPtrDtSurface( void ) const;
      virtual bool closed( dtInt const & dir ) const;
      virtual aFX invY(aFY const & yy) const;    
    private:
      ptrHandling<dtSurface> _dtS;
  };
  typedef analyticFunctionCompound<vec3dSurfaceTwoD> vec3dSurfaceTwoDCompound;
}
#endif	/* VEC3DSURFACETWOD_H */
