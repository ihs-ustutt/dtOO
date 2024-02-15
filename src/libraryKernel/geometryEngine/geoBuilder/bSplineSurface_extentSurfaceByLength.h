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

#ifndef bSplineSurface_extentSurfaceByLength_H
#define	bSplineSurface_extentSurfaceByLength_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class bSplineSurface_extentSurfaceByLength {
  public:
    dt__classOnlyName(bSplineSurface_extentSurfaceByLength);
    bSplineSurface_extentSurfaceByLength( 
      dtSurface const * const dtS, 
      dtInt const & parameterDir, dtReal const & length, 
      dtInt const & cont = 2, bool const & forward = false
    );
    virtual ~bSplineSurface_extentSurfaceByLength();
    dtSurface * result( void );
  private:
    ptrHandling< dtSurface > _dtS;
  };
}
#endif	/* bSplineSurface_extentSurfaceByLength_H */
