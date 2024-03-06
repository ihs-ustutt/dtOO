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

#ifndef GEOMCURVE2D_REPARAMINGEOMSURFACEOCC_H
#define	GEOMCURVE2D_REPARAMINGEOMSURFACEOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtCurve2d;
  class dtSurface;
  
  class geomCurve2d_reparamInGeomSurfaceOCC {
  public:
    dt__classOnlyName(geomCurve2d_reparamInGeomSurfaceOCC);    
    geomCurve2d_reparamInGeomSurfaceOCC();
    geomCurve2d_reparamInGeomSurfaceOCC(
      dtCurve const * const dtC, 
      dtSurface const * const dtS
    );
    virtual ~geomCurve2d_reparamInGeomSurfaceOCC();
    dtCurve2d * result( void );    
  private:
    dt__pH(dtCurve2d) _dtC2d;
  };
}
#endif	/* GEOMCURVE2D_REPARAMINGEOMSURFACEOCC_H */
