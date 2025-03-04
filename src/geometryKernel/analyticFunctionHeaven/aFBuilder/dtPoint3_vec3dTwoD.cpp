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

#include "dtPoint3_vec3dTwoD.h"

#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <logMe/dtMacros.h>

namespace dtOO {
dtPoint3_vec3dTwoD::dtPoint3_vec3dTwoD(
  vec3dTwoD const *const v3d, dtInt const &nU, dtInt const &nV
)
{
  _pp.resize(nU, nV);

  twoDArrayHandling<dtPoint2> uGrid = dtLinearAlgebra::unitGrid(nU, nV);
  dt__forAllIndex(uGrid, ii)
  {
    dt__forAllIndex(uGrid[ii], jj)
    {
      _pp[ii][jj] =
        v3d->YdtPoint3Percent(analyticFunction::aFXTwoD(uGrid[ii][jj]));
    }
  }
}

dtPoint3_vec3dTwoD::~dtPoint3_vec3dTwoD() {}

twoDArrayHandling<dtPoint3> dtPoint3_vec3dTwoD::result(void) { return _pp; }
} // namespace dtOO
