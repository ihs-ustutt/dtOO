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

#ifndef bSplineSurface_exchangeSurfaceConstructOCC_H
#define	bSplineSurface_exchangeSurfaceConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  
  class bSplineSurface_exchangeSurfaceConstructOCC {
  public:
    dt__classOnlyName(bSplineSurface_exchangeSurfaceConstructOCC);
    bSplineSurface_exchangeSurfaceConstructOCC( 
      dtSurface const * const dtS
    );
    virtual ~bSplineSurface_exchangeSurfaceConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling< dtSurface > _dtS;
  };
}
#endif	/* bSplineSurface_exchangeSurfaceConstructOCC_H */
