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

#include "vec3dTwoD_closeArithmetic.h"

#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
// #include "dtPoint3_vec3dTwoD.h"
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomSurface_closeGapsArithmeticOCC.h>

namespace dtOO {
vec3dTwoD_closeArithmetic::vec3dTwoD_closeArithmetic(
  dt__pVH(vec3dTwoD) const &v3d, dtInt const &dir, dtInt const &nSegments
)
{
  //
  // cast function to correct type
  //
  dt__pVH(dtSurface) dtS;
  dt__forAllConstIter(dt__pVH(vec3dTwoD), v3d, it)
  {
    vec3dTwoD const &theFunction = *it;
    dt__ptrAss(
      vec3dSurfaceTwoD const *const v3dS,
      vec3dSurfaceTwoD::ConstDownCast(&theFunction)
    );
    dtS.push_back(v3dS->ptrDtSurface()->clone());
  }

  //
  // close underlying surfaces
  //
  dtS = geomSurface_closeGapsArithmeticOCC(dtS, dir, nSegments).result();

  //
  // create new functions
  //
  dt__forAllIndex(dtS, ii)
  {
    dtSurface const &theSurface = dtS[ii];
    _v3d.push_back(new vec3dSurfaceTwoD(&theSurface));
    _v3d[ii].setLabel(v3d[ii].getLabel());
  }
}

vec3dTwoD_closeArithmetic::~vec3dTwoD_closeArithmetic() {}

dt__pVH(vec3dTwoD) vec3dTwoD_closeArithmetic::result(void) { return _v3d; }
} // namespace dtOO
