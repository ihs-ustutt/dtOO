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

#ifndef BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCC_H
#define	BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  
  class bezierSurface_bezierCurveFillConstructOCC {
    public:
      dt__classOnlyName(bezierSurface_bezierCurveFillConstructOCC);
      bezierSurface_bezierCurveFillConstructOCC( vectorHandling< dtCurve const * > const & cc );
      virtual ~bezierSurface_bezierCurveFillConstructOCC();
      dtSurface * result( void );
    private:
      ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* BEZIERSURFACE_BEZIERCURVEFILLCONSTRUCT_H */
