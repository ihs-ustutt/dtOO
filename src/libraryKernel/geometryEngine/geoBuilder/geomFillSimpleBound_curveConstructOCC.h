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

#ifndef geomFillSimpleBound_curveConstructOCC_H
#define	geomFillSimpleBound_curveConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  class dtCurve2d;
  
  class geomFillSimpleBound_curveConstructOCC {
  public:
    dt__classOnlyName(geomFillSimpleBound_curveConstructOCC);
    geomFillSimpleBound_curveConstructOCC( 
      dtCurve const * const dtC
    );
    virtual ~geomFillSimpleBound_curveConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* geomFillSimpleBound_curveConstructOCC_H */
