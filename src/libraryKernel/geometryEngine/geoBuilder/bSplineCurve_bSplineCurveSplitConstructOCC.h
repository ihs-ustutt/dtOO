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

#ifndef bSplineCurve_bSplineCurveSplitConstructOCC_H
#define	bSplineCurve_bSplineCurveSplitConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_bSplineCurveSplitConstructOCC {
    public:
      dt__classOnlyName(bSplineCurve_bSplineCurveSplitConstructOCC);
      bSplineCurve_bSplineCurveSplitConstructOCC( 
        dtCurve const * const dtC, dtReal const & u0, dtReal const & u1
      );
      virtual ~bSplineCurve_bSplineCurveSplitConstructOCC();
      dtCurve * result( void );
    private:
      dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_bSplineCurveSplitConstructOCC_H */
