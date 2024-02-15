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

#ifndef dtPoint3_vec3dTwoD_H
#define	dtPoint3_vec3dTwoD_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class vec3dTwoD;
  
  class dtPoint3_vec3dTwoD {
  public:
    dt__classOnlyName(dtPoint3_vec3dTwoD);    
    dtPoint3_vec3dTwoD(
      vec3dTwoD const * const v3d, dtInt const & nU, dtInt const & nV
    );
    virtual ~dtPoint3_vec3dTwoD();
    twoDArrayHandling< dtPoint3 > result( void );
  private:
    twoDArrayHandling< dtPoint3 > _pp;
  };
}
#endif	/* dtPoint3_vec3dTwoD_H */
