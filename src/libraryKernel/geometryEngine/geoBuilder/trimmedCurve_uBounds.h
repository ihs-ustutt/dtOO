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

#ifndef TRIMMEDCURVE_UBOUNDS_H
#define	TRIMMEDCURVE_UBOUNDS_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class trimmedCurve_uBounds {
    public:
      dt__classOnlyName(trimmedCurve_uBounds);
      trimmedCurve_uBounds(
        dtCurve const * dtC, dtReal const & min, dtReal const & max
      );
      virtual ~trimmedCurve_uBounds();
      dtCurve * result( void );
      private:
        ptrHandling< dtCurve > _dtC;
  };
}
#endif	/* TRIMMEDCURVE_UBOUNDS_H */
