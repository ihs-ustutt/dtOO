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

#include "vec2dCurve2dOneD.h"
#include "dtLinearAlgebra.h"

#include <discrete2dPoints.h>
#include <geometryEngine/dtCurve2d.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <solid2dLine.h>

#include "analyticFunctionTransformed.h"

namespace dtOO {
vec2dCurve2dOneD::vec2dCurve2dOneD() : vec2dOneD() {}

vec2dCurve2dOneD::vec2dCurve2dOneD(const vec2dCurve2dOneD &orig)
  : vec2dOneD(orig)
{
  _dtC2d.reset(orig._dtC2d->clone());
}

vec2dCurve2dOneD::~vec2dCurve2dOneD() {}

vec2dCurve2dOneD::vec2dCurve2dOneD(dtCurve2d const *const dtC2d) : vec2dOneD()
{
  _dtC2d.reset(dtC2d->clone());
  setMin(dtC2d->minU());
  setMax(dtC2d->maxU());
}

aFY vec2dCurve2dOneD::Y(dtReal const &xx) const
{
  dtPoint2 p2d = _dtC2d->point(xx);

  aFY yy(2, 0.);

  yy[0] = p2d.x();
  yy[1] = p2d.y();

  return yy;
}

vec2dCurve2dOneD *vec2dCurve2dOneD::clone(void) const
{
  return new vec2dCurve2dOneD(*this);
}

vec2dCurve2dOneD *
vec2dCurve2dOneD::cloneTransformed(dtTransformer const *const dtT) const
{
  return new analyticFunctionTransformed<vec2dCurve2dOneD>(*this, dtT);
}

vec2dCurve2dOneD *vec2dCurve2dOneD::create(void) const
{
  return new vec2dCurve2dOneD();
}

vectorHandling<renderInterface *> vec2dCurve2dOneD::getRender(void) const
{
  int nU = staticPropertiesHandler::getInstance()->getOptionInt(
    "function_render_resolution_u"
  );

  vectorHandling<dtPoint2> p2(nU);
  dtReal interval = (xMax(0) - xMin(0)) / (nU - 1);
  for (int ii = 0; ii < nU; ii++)
  {
    dtReal iiF = static_cast<dtReal>(ii);
    dtReal xx = xMin(0) + iiF * interval;
    p2[ii] = YdtPoint2(xx);
  }

  vectorHandling<dtPoint2> p2Cp(_dtC2d->nControlPoints());
  for (int ii = 0; ii < p2Cp.size(); ii++)
  {
    p2Cp[ii] = _dtC2d->controlPoint(ii);
  }

  vectorHandling<renderInterface *> rV(2);
  rV[0] = new solid2dLine(p2);
  rV[1] = new discrete2dPoints(p2Cp);

  return rV;
}

dtCurve2d const *vec2dCurve2dOneD::ptrDtCurve2d(void) const
{
  return _dtC2d.get();
}

bool vec2dCurve2dOneD::closed(dtInt const &dir) const
{
  switch (dir)
  {
  case 0:
    return static_cast<bool>(_dtC2d->closed());
  default:
    dt__throw(closed(), << dt__eval(dir) << std::endl << "dim should be 0.");
  }
}
} // namespace dtOO
