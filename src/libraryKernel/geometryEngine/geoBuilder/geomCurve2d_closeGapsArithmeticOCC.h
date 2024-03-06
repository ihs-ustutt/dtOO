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

#ifndef geomCurve2d_closeGapsArithmeticOCC_H
#define	geomCurve2d_closeGapsArithmeticOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
//  class dtCurve;
  class dtCurve2d;
//  class dtSurface;
  
  class geomCurve2d_closeGapsArithmeticOCC {
  public:
    dt__classOnlyName(geomCurve2d_closeGapsArithmeticOCC);    
    geomCurve2d_closeGapsArithmeticOCC();
    geomCurve2d_closeGapsArithmeticOCC(dt__pVH(dtCurve2d) const & dtC2d);
    virtual ~geomCurve2d_closeGapsArithmeticOCC();
    dt__pVH(dtCurve2d) result( void );    
  private:
    dt__pVH(dtCurve2d) _dtC2d;
  };
}
#endif	/* geomCurve2d_closeGapsArithmeticOCC_H */
