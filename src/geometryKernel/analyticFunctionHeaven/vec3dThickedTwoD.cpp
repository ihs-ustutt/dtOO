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

#include "vec3dThickedTwoD.h"
#include <dtLinearAlgebra.h>

#include "scaTwoD.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
vec3dThickedTwoD::vec3dThickedTwoD() : vec3dTwoD() {}

vec3dThickedTwoD::vec3dThickedTwoD(const vec3dThickedTwoD &orig)
  : vec3dTwoD(orig)
{
  _ff.reset(orig._ff->clone());
  _tt.reset(orig._tt->clone());
  _nf = orig._nf;
  setMin(0, _tt->xMin(0));
  setMin(1, _tt->xMin(1));
  setMax(0, _tt->xMax(0));
  setMax(1, _tt->xMax(1));
}

vec3dThickedTwoD::~vec3dThickedTwoD() {}

vec3dThickedTwoD::vec3dThickedTwoD(
  vec3dTwoD const *const ff, vec3dTwoD const *const tt, dtVector3 const nf
)
  : vec3dTwoD()
{
  _ff.reset(ff->clone());
  _tt.reset(tt->clone());
  _nf = nf;
  setMin(0, _tt->xMin(0));
  setMin(1, _tt->xMin(1));
  setMax(0, _tt->xMax(0));
  setMax(1, _tt->xMax(1));
}

aFY vec3dThickedTwoD::Y(aFX const &xx) const
{
  dtPoint3 ttUV = _tt->YdtPoint3(xx);
  dtReal tt = ttUV.x();
  aFX uvT(2, 0.);
  uvT[0] = ttUV.y();
  uvT[1] = ttUV.z();

  dtPoint3 yy = _ff->YdtPoint3Percent(uvT);
  dtVector3 nn = _ff->unitNdtVector3Percent(uvT);
  nn = nn - dtLinearAlgebra::dotProduct(nn, _nf) * _nf;
  dtLinearAlgebra::normalize(nn);

  dtPoint3 p3 = yy + tt * nn;

  aFY retY(3, 0.);
  retY[0] = p3.x();
  retY[1] = p3.y();
  retY[2] = p3.z();
  return retY;
}

vec3dThickedTwoD *vec3dThickedTwoD::clone(void) const
{
  return new vec3dThickedTwoD(*this);
}

vec3dThickedTwoD *
vec3dThickedTwoD::cloneTransformed(dtTransformer const *const dtT) const
{
  return new analyticFunctionTransformed<vec3dThickedTwoD>(*this, dtT);
}

vec3dThickedTwoD *vec3dThickedTwoD::create(void) const
{
  return new vec3dThickedTwoD();
}

/**
 * @todo Is this correct? Is it never closed?
 */
bool vec3dThickedTwoD::closed(dtInt const &dir) const { return false; }
} // namespace dtOO
