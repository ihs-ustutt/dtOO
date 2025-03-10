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

#include "scaTwoD.h"
#include <discrete2dPoints.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <solid2dLine.h>

namespace dtOO {
scaTwoD::scaTwoD() : scaFunction() {}

scaTwoD::scaTwoD(scaTwoD const &orig) : scaFunction(orig)
{
  _min[0] = orig._min[0];
  _min[1] = orig._min[1];
  _max[0] = orig._max[0];
  _max[1] = orig._max[1];
}

scaTwoD::~scaTwoD() {}

dtReal scaTwoD::YFloat(aFX const &xx) const
{
  dt__throwIf(xx.size() != 2, YFloat());

  return YFloat(xx[0], xx[1]);
}

dtReal scaTwoD::YFloatPercent(dtReal const &xP0, dtReal const &xP1) const
{
  return YFloat(x_percent(xP0, xP1));
}

dtInt scaTwoD::xDim(void) const { return 2; }

dtReal scaTwoD::xMin(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _min[0];
  case 1:
    return _min[1];
  default:
    dt__throw(xMin(), << dt__eval(dir) << std::endl << "dir should be 0 or 1.");
  }
}

dtReal scaTwoD::xMax(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return _max[0];
  case 1:
    return _max[1];
  default:
    dt__throw(xMax(), << dt__eval(dir) << std::endl << "dir should be 0 or 1.");
  }
}

void scaTwoD::setMin(int const &dir, dtReal const &min)
{
  switch (dir)
  {
  case 0:
    _min[0] = min;
    return;
  case 1:
    _min[1] = min;
    return;
  default:
    dt__throw(
      setMin(),
      << dt__eval(dir) << std::endl
      << "dir should be 0 or 1."
    );
  }
}

void scaTwoD::setMax(int const &dir, dtReal const &max)
{
  switch (dir)
  {
  case 0:
    _max[0] = max;
    return;
  case 1:
    _max[1] = max;
    return;
  default:
    dt__throw(
      setMax(),
      << dt__eval(dir) << std::endl
      << "dir should be 0 or 1."
    );
  }
}

aFX scaTwoD::x_percent(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0.);
  xx[0] = x0;
  xx[1] = x1;

  return x_percent(xx);
}

aFX scaTwoD::percent_x(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0.);
  xx[0] = x0;
  xx[1] = x1;

  return percent_x(xx);
}

std::vector<dtReal> scaTwoD::DYFloat(aFX const &xx) const
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

  twoDArrayHandling<dtReal> yy(2, 2);
  yy[0][0] = YFloat(x_percent(uv[0][0], xP[1]));
  yy[0][1] = YFloat(x_percent(uv[0][1], xP[1]));
  yy[1][0] = YFloat(x_percent(xP[0], uv[1][0]));
  yy[1][1] = YFloat(x_percent(xP[0], uv[1][1]));

  std::vector<dtReal> dxdy(2);
  dxdy[0] = (yy[0][1] - yy[0][0]) /
            (x_percent(uv[0][1], xP[1])[0] - x_percent(uv[0][0], xP[1])[0]);
  dxdy[1] = (yy[1][1] - yy[1][0]) /
            (x_percent(xP[0], uv[1][1])[1] - x_percent(xP[0], uv[1][0])[1]);

  return dxdy;
}

std::vector<dtReal> scaTwoD::DYFloat(dtReal const &x0, dtReal const &x1) const
{
  aFX xx(2, 0);
  xx[0] = x0;
  xx[1] = x1;

  return DYFloat(xx);
}

vectorHandling<renderInterface *> scaTwoD::getRender(void) const
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

  for (int jj = 0; jj < nV; jj++)
  {
    dt__toFloat(dtReal jjF, jj);
    dtReal constV = xMin(1) + jjF * intervalV;
    for (int ii = 0; ii < nU; ii++)
    {
      dt__toFloat(dtReal iiF, ii);
      aFX xx(2, 0);
      xx[0] = xMin(0) + iiF * intervalU;
      xx[1] = constV;
      dtPoint2 p2tmp = dtPoint2(xx[0], YFloat(xx));
      p2.push_back(dtPoint2(p2tmp.x(), p2tmp.y()));
    }
    rV.push_back(new solid2dLine(p2));
    p2.clear();
  }
  return rV;
}

dt__C_addCloneForpVH(scaTwoD);
} // namespace dtOO
