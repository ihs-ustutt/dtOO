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

#ifndef x_vec3dTwoDClosestPointToPoint_H
#define	x_vec3dTwoDClosestPointToPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>
#include <analyticFunctionHeaven/analyticFunction.h>

namespace dtOO {
  class vec3dTwoD;
  
  class x_vec3dTwoDClosestPointToPoint {
  public:
    dt__classOnlyName(x_vec3dTwoDClosestPointToPoint);     
    x_vec3dTwoDClosestPointToPoint(
      vec3dTwoD const * const v3d, aFY const & yy
    );
    virtual ~x_vec3dTwoDClosestPointToPoint();
    aFX result( void );
  private:
    double F(double const * xx) const;    
  private:
    aFX _xx;
    vec3dTwoD const & _v3d;
    aFY const & _yy;
  };
}
#endif	/* x_vec3dTwoDClosestPointToPoint_H */
