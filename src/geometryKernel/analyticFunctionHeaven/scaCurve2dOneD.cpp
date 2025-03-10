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

#include "scaCurve2dOneD.h"
#include "analyticFunctionTransformed.h"

#include <attributionHeaven/pointCurve2dOneDDist.h>
#include <geometryEngine/dtCurve2d.h>
#include <gslMinFloatAttr.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <solid2dLine.h>

namespace dtOO {
scaCurve2dOneD::scaCurve2dOneD() : scaOneD() {}

scaCurve2dOneD::scaCurve2dOneD(scaCurve2dOneD const &orig) : scaOneD(orig)
{
  _dtC2d.reset(orig._dtC2d->clone());
  scaOneD::setMinMax(orig.xMin(0), orig.xMax(0));
}

scaCurve2dOneD::scaCurve2dOneD(dtCurve2d const *const orig) : scaOneD()
{

  _dtC2d.reset(orig->clone());

  dtPoint2 pp = _dtC2d->pointPercent(0.);

  scaOneD::setMinMax(pp.x(), pp.x());

  pp = _dtC2d->pointPercent(1.);

  if (pp.x() < scaOneD::xMax(0))
  {
    scaOneD::setMin(pp.x());
  }
  else
  {
    scaOneD::setMax(pp.x());
  }
}

scaCurve2dOneD *scaCurve2dOneD::clone(void) const
{
  return new scaCurve2dOneD(*this);
}

scaCurve2dOneD *scaCurve2dOneD::cloneTransformed(dtTransformer const *const dtT
) const
{
  return new analyticFunctionTransformed<scaCurve2dOneD>(*this, dtT);
}

scaCurve2dOneD *scaCurve2dOneD::create(void) const
{
  return new scaCurve2dOneD();
}

scaCurve2dOneD::~scaCurve2dOneD() {}

dtReal scaCurve2dOneD::YFloat(dtReal const &xx) const
{
  bool mustIterate = true;
  dtReal theRoot;
  if ((xx > xMax(0)) || (xx < xMin(0)))
  {
    dtReal const range = fabs(xMax(0) - xMin(0));
    if (floatHandling::isSmall((xMin(0) - xx) / range))
    {
      mustIterate = false;
      theRoot = _dtC2d->minU();
    }
    else if (floatHandling::isSmall((xMax(0) - xx) / range))
    {
      mustIterate = false;
      theRoot = _dtC2d->maxU();
    }
    else
    {
      dt__throw(
        YFloat(),
        << dt__eval(getLabel()) << std::endl
        << dt__eval(xx) << std::endl
        << dt__eval(xMin(0)) << std::endl
        << dt__eval(xMax(0)) << std::endl
        << "xx out of range."
      );
    }
  }
  if (mustIterate)
  {
    gslMinFloatAttr md(
      dt__pH(pointCurve2dOneDDist)(new pointCurve2dOneDDist(xx, _dtC2d.get(), 0)
      ),
      0.5,
      0.001,
      staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution")
    );
    md.perform();
    dt__throwIf(!md.converged(), YFloat());
    theRoot = _dtC2d->u_uPercent(md.result()[0]);
  }

  //
  // output
  //
  dtPoint2 pp = _dtC2d->point(static_cast<dtReal>(theRoot));
  //		dt__info(
  //			YFloat(),
  //			<< dt__eval(mustIterate) << std::endl
  //			<< dt__eval(xx) << std::endl
  //			<< dt__eval(fabs(xx-pp.x())) << std::endl
  //		  << dt__point2d(pp) << std::endl
  //			<< getLabel() << "(" << pp.x() << ") = " << pp.y()
  //		);

  return pp.y();
}

vectorHandling<renderInterface *> scaCurve2dOneD::getRender(void) const
{
  int nU = staticPropertiesHandler::getInstance()->getOptionInt(
    "function_render_resolution_u"
  );

  vectorHandling<dtPoint2> p2(nU);
  dtReal interval = (_dtC2d->maxU() - _dtC2d->minU()) / (nU - 1);
  for (int ii = 0; ii < nU; ii++)
  {
    dtReal iiF = static_cast<dtReal>(ii);
    p2[ii] = _dtC2d->point(_dtC2d->minU() + iiF * interval);
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

dtCurve2d const *scaCurve2dOneD::constPtrDtCurve2d(void) const
{
  return _dtC2d.get();
}

dtCurve2d *scaCurve2dOneD::ptrDtCurve2d(void) { return _dtC2d.get(); }

void scaCurve2dOneD::translate(dtVector2 const vector)
{
  //
  // perform translation
  //
  _dtC2d->translate(vector);

  //
  // adjust range
  //
  scaOneD::setMinMax(
    _dtC2d->controlPoint(0).x(), _dtC2d->lastControlPoint().x()
  );
}

std::vector<dtPoint2> scaCurve2dOneD::getControlPoints(void) const
{
  std::vector<dtPoint2> cP;

  for (int ii = 0; ii < _dtC2d->nControlPoints(); ii++)
  {
    cP.push_back(_dtC2d->controlPoint(ii));
  }

  return cP;
}
} // namespace dtOO
