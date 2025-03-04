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

#ifndef TRANSINTCUBE_H
#define TRANSINTCUBE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class map2dTo3d;

class transIntCube {
public:
  dt__classOnlyName(transIntCube);
  transIntCube();
  transIntCube(
    map2dTo3d const *const aS0,
    map2dTo3d const *const aS1,
    map2dTo3d const *const aS2,
    map2dTo3d const *const aS3,
    map2dTo3d const *const aS4,
    map2dTo3d const *const aS5
  );
  transIntCube(const transIntCube &orig);
  virtual ~transIntCube();
  dtPoint3 getValue(dtReal const xx, dtReal const yy, dtReal const zz) const;
  vectorHandling<map2dTo3d const *> const &getConstRefToMap2dTo3d(void) const;

private:
  vectorHandling<map2dTo3d const *> _aS;
  dtVector3 _aS_1_0_0;
  dtVector3 _aS_1_0_1;
  dtVector3 _aS_1_1_0;
  dtVector3 _aS_1_1_1;
  dtVector3 _aS_3_0_0;
  dtVector3 _aS_3_0_1;
  dtVector3 _aS_3_1_0;
  dtVector3 _aS_3_1_1;
};
} // namespace dtOO
#endif /* TRANSINTCUBE_H */
