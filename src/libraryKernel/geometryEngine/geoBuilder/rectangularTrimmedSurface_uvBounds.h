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

#ifndef RECTANGULARTRIMMEDSURFACE_UVBOUNDS_H
#define	RECTANGULARTRIMMEDSURFACE_UVBOUNDS_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class rectangularTrimmedSurface_uvBounds {
  public:
    dt__classOnlyName(rectangularTrimmedSurface_uvBounds);
    rectangularTrimmedSurface_uvBounds(
      dtSurface const * dtS, dtPoint2 const & min, dtPoint2 const & max
    );
    virtual ~rectangularTrimmedSurface_uvBounds();
    dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* RECTANGULARTRIMMEDSURFACE_UVBOUNDS_H */
