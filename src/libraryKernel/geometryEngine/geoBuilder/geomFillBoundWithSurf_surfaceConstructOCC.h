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

#ifndef geomFillBoundWithSurf_surfaceConstructOCC_H
#define	geomFillBoundWithSurf_surfaceConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  class dtCurve2d;
  
  class geomFillBoundWithSurf_surfaceConstructOCC {
  public:
    dt__classOnlyName(geomFillBoundWithSurf_surfaceConstructOCC);
    geomFillBoundWithSurf_surfaceConstructOCC( 
      dtSurface const * const dtS, dtCurve2d const * const dtC2d
    );
    virtual ~geomFillBoundWithSurf_surfaceConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* geomFillBoundWithSurf_surfaceConstructOCC_H */
