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

#include "vec2dTwoD.h"
#include <discrete2dPoints.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <solid2dLine.h>

namespace dtOO {
vec2dTwoD::vec2dTwoD() : vec2dFunction() {}

vec2dTwoD::vec2dTwoD(const vec2dTwoD &orig) : vec2dFunction(orig)
{
  _min[0] = orig._min[0];
  _min[1] = orig._min[1];
  _max[0] = orig._max[0];
  _max[1] = orig._max[1];
}

vec2dTwoD::~vec2dTwoD() {}

int vec2dTwoD::xDim(void) const { return 2; }

aFY vec2dTwoD::Y(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);

  xx[0] = x0;
  xx[1] = x1;

  return Y(xx);
}

dtPoint2 vec2dTwoD::YdtPoint2(aFX const &xx) const
{
  return vec2dFunction::YdtPoint2(xx);
}

dtPoint2 vec2dTwoD::YdtPoint2(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);

  xx[0] = x0;
  xx[1] = x1;

  return vec2dFunction::YdtPoint2(xx);
}

dtPoint2 vec2dTwoD::YdtPoint2Percent(aFX const &xx) const
{
  return YdtPoint2(x_percent(xx));
}

dtPoint2 vec2dTwoD::YdtPoint2Percent(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);

  xx[0] = x0;
  xx[1] = x1;

  return YdtPoint2(x_percent(xx));
}

aFX vec2dTwoD::x_percent(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0.);
  xx[0] = x0;
  xx[1] = x1;

  return x_percent(xx);
}

aFX vec2dTwoD::percent_x(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0.);
  xx[0] = x0;
  xx[1] = x1;

  return percent_x(xx);
}

std::vector<dtVector2> vec2dTwoD::DYdtVector2(aFX const &xx) const
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

  twoDArrayHandling<dtPoint2> yy(2, 2);
  yy[0][0] = YdtPoint2(x_percent(uv[0][0], xP[1]));
  yy[0][1] = YdtPoint2(x_percent(uv[0][1], xP[1]));
  yy[1][0] = YdtPoint2(x_percent(xP[0], uv[1][0]));
  yy[1][1] = YdtPoint2(x_percent(xP[0], uv[1][1]));

  std::vector<dtVector2> dxdy(2);
  dxdy[0] = (yy[0][1] - yy[0][0]) /
            (x_percent(uv[0][1], xP[1])[0] - x_percent(uv[0][0], xP[1])[0]);
  dxdy[1] = (yy[1][1] - yy[1][0]) /
            (x_percent(xP[0], uv[1][1])[1] - x_percent(xP[0], uv[1][0])[1]);

  return dxdy;
}

std::vector<dtVector2>
vec2dTwoD::DYdtVector2(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);
  xx[0] = x0;
  xx[1] = x1;

  return DYdtVector2(xx);
}

dtReal vec2dTwoD::xMin(dtInt const &dir) const
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

dtReal vec2dTwoD::xMax(dtInt const &dir) const
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

void vec2dTwoD::setMin(int const &dir, dtReal const &min)
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

void vec2dTwoD::setMax(int const &dir, dtReal const &max)
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

vectorHandling<renderInterface *> vec2dTwoD::getRender(void) const
{
  int nU = staticPropertiesHandler::getInstance()->getOptionInt(
    "function_render_resolution_u"
  );
  int nV = staticPropertiesHandler::getInstance()->getOptionInt(
    "function_render_resolution_v"
  );
  vectorHandling<renderInterface *> rV; //(nV);

  vectorHandling<dtPoint2> p2;
  dtReal intervalU = (xMax(0) - xMin(0)) / (nU - 1);
  dtReal intervalV = (xMax(1) - xMin(1)) / (nV - 1);

  for (int ii = 0; ii < nU; ii++)
  {
    dt__toFloat(dtReal iiF, ii);
    aFX xx(2, 0);
    xx[0] = xMin(0) + iiF * intervalU;
    xx[1] = xMin(1);
    dtPoint2 p2tmp = YdtPoint2(xx);
    //			dt__info(getRender(), << dt__point3d(p3) );
    p2.push_back(dtPoint2(p2tmp.x(), p2tmp.y()));
  }
  rV.push_back(new solid2dLine(p2));
  p2.clear();
  for (int jj = 0; jj < nV; jj++)
  {
    dt__toFloat(dtReal jjF, jj);
    aFX xx(2, 0);
    xx[0] = xMin(0);
    xx[1] = xMin(1) + jjF * intervalV;
    dtPoint2 p2tmp = YdtPoint2(xx);
    //			dt__info(getRender(), << dt__point3d(p3) );
    p2.push_back(dtPoint2(p2tmp.x(), p2tmp.y()));
  }
  rV.push_back(new solid2dLine(p2));
  p2.clear();
  for (int ii = 0; ii < nU; ii++)
  {
    dt__toFloat(dtReal iiF, ii);
    aFX xx(2, 0);
    xx[0] = xMin(0) + iiF * intervalU;
    xx[1] = xMax(1);
    dtPoint2 p2tmp = YdtPoint2(xx);
    //			dt__info(getRender(), << dt__point3d(p3) );
    p2.push_back(dtPoint2(p2tmp.x(), p2tmp.y()));
  }
  rV.push_back(new solid2dLine(p2));
  p2.clear();
  for (int jj = 0; jj < nV; jj++)
  {
    dt__toFloat(dtReal jjF, jj);
    aFX xx(2, 0);
    xx[0] = xMax(0);
    xx[1] = xMin(1) + jjF * intervalV;
    dtPoint2 p2tmp = YdtPoint2(xx);
    //			dt__info(getRender(), << dt__point3d(p3) );
    p2.push_back(dtPoint2(p2tmp.x(), p2tmp.y()));
  }
  rV.push_back(new solid2dLine(p2));

  return rV;
}

dt__C_addCloneForpVH(vec2dTwoD);
} // namespace dtOO
