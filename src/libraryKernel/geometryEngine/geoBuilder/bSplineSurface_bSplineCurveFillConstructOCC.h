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

#ifndef BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H
#define	BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bSplineSurface_bSplineCurveFillConstructOCC {
    public:
      dt__classOnlyName(bSplineSurface_bSplineCurveFillConstructOCC);
      bSplineSurface_bSplineCurveFillConstructOCC( 
        vectorHandling< dtCurve const * > const & cc 
      );
      bSplineSurface_bSplineCurveFillConstructOCC( 
        dt__pVH(dtCurve) const & cc 
      );
      bSplineSurface_bSplineCurveFillConstructOCC( 
        dtCurve const * const c0, dtCurve const * const c1, 
        dtCurve const * const c2, dtCurve const * const c3
      );      
      virtual ~bSplineSurface_bSplineCurveFillConstructOCC();
      dtSurface * result( void );
    private:
      dt__pH(dtSurface) _dtS;
  };
}
#endif	/* BSPLINESURFACE_BSPLINECURVEFILLCONSTRUCTOCC_H */
