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

#ifndef bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC_H
#define	bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC {
  public:
    dt__classOnlyName(
      bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC
    );
    bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC( 
      dtReal angleIn, dtReal angleOut, dtReal const & deltaX, dtReal const & deltaY, 
      dtReal const & pointOne, dtReal const & pointTwo
    );
    virtual ~bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC_H */
