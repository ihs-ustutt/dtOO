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

#ifndef RECTANGULARTRIMMEDSURFACE_CURVEROTATECONSTRUCTOCC_H
#define	RECTANGULARTRIMMEDSURFACE_CURVEROTATECONSTRUCTOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class rectangularTrimmedSurface_curveRotateConstructOCC {
  public:
    dt__classOnlyName(rectangularTrimmedSurface_curveRotateConstructOCC);  
    rectangularTrimmedSurface_curveRotateConstructOCC( 
      dtCurve const & cL, 
      dtPoint3 const & oP, 					
      dtVector3 const & rA, 
      dtReal const & angle 
    );
    virtual ~rectangularTrimmedSurface_curveRotateConstructOCC();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* RECTANGULARTRIMMEDSURFACE_CURVEROTATECONSTRUCTOCC_H */
