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

#include "vec3dTwoD.h"
#include "analyticFunctionHeaven/analyticFunction.h"
#include <discrete2dPoints.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <solid3dSurface.h>

namespace dtOO {
vec3dTwoD::vec3dTwoD() : vec3dFunction() {}

vec3dTwoD::vec3dTwoD(const vec3dTwoD &orig) : vec3dFunction(orig)
{
  _min[0] = orig._min[0];
  _min[1] = orig._min[1];
  _max[0] = orig._max[0];
  _max[1] = orig._max[1];
}

vec3dTwoD::~vec3dTwoD() {}

int vec3dTwoD::xDim(void) const { return 2; }

aFY vec3dTwoD::Y(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);

  xx[0] = x0;
  xx[1] = x1;

  return Y(xx);
}

dtPoint3 vec3dTwoD::YdtPoint3(aFX const &xx) const
{
  return vec3dFunction::YdtPoint3(xx);
}

dtPoint3 vec3dTwoD::YdtPoint3(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);

  xx[0] = x0;
  xx[1] = x1;

  return vec3dFunction::YdtPoint3(xx);
}

dtPoint3 vec3dTwoD::YdtPoint3Percent(aFX const &xx) const
{
  return YdtPoint3(x_percent(xx));
}

dtPoint3 vec3dTwoD::YdtPoint3Percent(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);

  xx[0] = x0;
  xx[1] = x1;

  return YdtPoint3(x_percent(xx));
}

aFX vec3dTwoD::x_percent(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0.);
  xx[0] = x0;
  xx[1] = x1;

  return x_percent(xx);
}

aFX vec3dTwoD::percent_x(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0.);
  xx[0] = x0;
  xx[1] = x1;

  return percent_x(xx);
}

std::vector<dtVector3> vec3dTwoD::DYdtVector3(aFX const &xx) const
{
  /*
   *                      (xP[0], uv[1][1]) -> yy[1][1]
   *                               o
   *                               |
   *                               |
   *                               |
   *  (uv[0][0], xP[1]) o---------------------o (uv[0][1], xP[1])
   *     -> yy[0][0]               |               -> yy[0][1]
   *                               |
   *                               |
   *                               o
   *                      (xP[0], uv[1][0]) -> yy[1][0]
   */
  aFX xP = percent_x(xx);
  dtReal deltaPer[2];
  deltaPer[0] = 0.0001;
  deltaPer[1] = 0.0001;

  std::vector<aFX> uv(2, aFX(2, 0.));
  for (int ii = 0; ii < 2; ii++)
  {
    if (xP[ii] < deltaPer[ii])
    {
      uv[ii][0] = 0.;
      uv[ii][1] = deltaPer[ii];
    }
    else if ((xP[ii] >= deltaPer[ii]) && (xP[ii] <= (1. - deltaPer[ii])))
    {
      uv[ii][0] = xP[ii] - deltaPer[ii];
      uv[ii][1] = xP[ii] + deltaPer[ii];
    }
    else if (xP[ii] > (1. - deltaPer[ii]))
    {
      uv[ii][0] = 1. - deltaPer[ii];
      uv[ii][1] = 1.;
    }
  }

  twoDArrayHandling<dtPoint3> yy(2, 2);
  yy[0][0] = YdtPoint3(x_percent(uv[0][0], xP[1]));
  yy[0][1] = YdtPoint3(x_percent(uv[0][1], xP[1]));
  yy[1][0] = YdtPoint3(x_percent(xP[0], uv[1][0]));
  yy[1][1] = YdtPoint3(x_percent(xP[0], uv[1][1]));

  std::vector<dtVector3> dxdy(2);
  dxdy[0] = (yy[0][1] - yy[0][0]) /
            (x_percent(uv[0][1], xP[1])[0] - x_percent(uv[0][0], xP[1])[0]);
  dxdy[1] = (yy[1][1] - yy[1][0]) /
            (x_percent(xP[0], uv[1][1])[1] - x_percent(xP[0], uv[1][0])[1]);

  return dxdy;
}

std::vector<dtVector3>
vec3dTwoD::DYdtVector3(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);
  xx[0] = x0;
  xx[1] = x1;

  return DYdtVector3(xx);
}

dtVector3 vec3dTwoD::unitNdtVector3(aFX const &xx) const
{
  std::vector<dtVector3> dxdy = DYdtVector3(xx);

  dtVector3 nn = dtLinearAlgebra::crossProduct(dxdy[0], dxdy[1]);

  return dtLinearAlgebra::normalize(nn);
}

dtVector3 vec3dTwoD::unitNdtVector3Percent(aFX const &xx) const
{
  return unitNdtVector3(x_percent(xx));
}

dtReal vec3dTwoD::xMin(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _min[0];
    break;
  case 1:
    return _min[1];
    break;
  default:
    dt__throw(xMin(), << dt__eval(dir) << std::endl << "dir should be 0 or 1.");
  }
}

dtReal vec3dTwoD::xMax(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _max[0];
    break;
  case 1:
    return _max[1];
    break;
  default:
    dt__throw(xMax(), << dt__eval(dir) << std::endl << "dir should be 0 or 1.");
  }
}

void vec3dTwoD::setMin(int const &dir, dtReal const &min)
{
  switch (dir)
  {
  case 0:
    _min[0] = min;
    break;
  case 1:
    _min[1] = min;
    break;
  default:
    dt__throw(
      setMin(),
      << dt__eval(dir) << std::endl
      << "dir should be 0 or 1."
    );
  }
}

void vec3dTwoD::setMax(int const &dir, dtReal const &max)
{
  switch (dir)
  {
  case 0:
    _max[0] = max;
    break;
  case 1:
    _max[1] = max;
    break;
  default:
    dt__throw(
      setMax(),
      << dt__eval(dir) << std::endl
      << "dir should be 0 or 1."
    );
  }
}

vectorHandling<renderInterface *> vec3dTwoD::getRender(void) const
{
  int nU = staticPropertiesHandler::getInstance()->getOptionInt(
    "function_render_resolution_u"
  );
  int nV = staticPropertiesHandler::getInstance()->getOptionInt(
    "function_render_resolution_v"
  );
  vectorHandling<renderInterface *> rV; //(nV);

  twoDArrayHandling<dtPoint3> pp(nU, nV);
  pp.resize(nU, nV);

  twoDArrayHandling<dtPoint2> uGrid = dtLinearAlgebra::unitGrid(nU, nV);
  dt__forAllIndex(uGrid, ii)
  {
    dt__forAllIndex(uGrid[ii], jj)
    {
      pp[ii][jj] =
        this->YdtPoint3Percent(analyticFunction::aFXTwoD(uGrid[ii][jj]));
    }
  }

  rV.push_back(new solid3dSurface(pp));

  return rV;
}

aFX vec3dTwoD::operator%(const aFX &percent) const
{
  return x_percent(percent);
}

dt__C_addCloneForpVH(vec3dTwoD);
} // namespace dtOO
