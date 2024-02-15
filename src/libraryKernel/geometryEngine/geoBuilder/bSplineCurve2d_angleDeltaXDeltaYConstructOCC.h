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

#ifndef BSPLINECURVE2D_ANGLEDELTAXDELTAYCONSTRUCTOCC_H
#define	BSPLINECURVE2D_ANGLEDELTAXDELTAYCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_angleDeltaXDeltaYConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve2d_angleDeltaXDeltaYConstructOCC);
    bSplineCurve2d_angleDeltaXDeltaYConstructOCC( 
      dtReal angleIn, dtReal angleOut, dtReal const & deltaX, dtReal const & deltaY
    );
    virtual ~bSplineCurve2d_angleDeltaXDeltaYConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* BSPLINECURVE2D_ANGLEDELTAXDELTAYCONSTRUCTOCC_H */
