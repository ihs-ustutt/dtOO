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

#ifndef BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H
#define	BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_pointConstructArcLengthParaOCC {
  public:
    dt__classOnlyName(bSplineCurve_pointConstructArcLengthParaOCC);
    bSplineCurve_pointConstructArcLengthParaOCC( 
      dtCurve const * dtC, dtReal const tol, 
      dtInt const maxOrder, dtInt const segments       
    );
    virtual ~bSplineCurve_pointConstructArcLengthParaOCC();
    dtCurve * result( void );
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* BSPLINECURVE_POINTCONSTRUCTARCLENGTHPARAOCC_H */
