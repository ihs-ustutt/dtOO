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

#include "dtCurve.h"

#include "geoBuilder/u_geomCurveClosestPoint.h"
#include <logMe/logMe.h>

namespace dtOO {
dtCurve::dtCurve() {}

dtCurve::~dtCurve() {}

dtReal dtCurve::reparam(dtPoint3 const ppXYZ) const
{
  return u_geomCurveClosestPoint(this, ppXYZ).result();
}

//
// optional overload
//
dtReal dtCurve::getUMin(void) const { return minPara(0); }

dtReal dtCurve::getUMax(void) const { return maxPara(0); }

dtReal dtCurve::u_uPercent(dtReal const percent) const
{
  return minPara(0) + percent * (maxPara(0) - minPara(0));
}

dtReal dtCurve::uPercent_u(dtReal const uu) const
{
  dtReal uMin = minPara(0);
  dtReal uMax = maxPara(0);

  return (uu - uMin) / (uMax - uMin);
}

dtPoint3 dtCurve::pointPercent(dtReal const percent) const
{
  return point(u_uPercent(percent));
}

dtVector3 dtCurve::firstDerPercent(dtReal const percent) const
{
  return firstDer(u_uPercent(percent));
}

dtReal dtCurve::length(void) const { return l_u(maxPara(0)); }

dtReal dtCurve::l_uPercent(dtReal const percent) const
{
  return l_u(u_uPercent(percent));
}

dtReal dtCurve::lPercent_u(dtReal const uu) const { return l_u(uu) / length(); }

dtReal dtCurve::lPercent_uPercent(dtReal const uuPercent) const
{
  return l_uPercent(uuPercent) / length();
}

dtReal dtCurve::u_lPercent(dtReal const percent) const
{
  return u_l(percent * length());
}

dtReal dtCurve::uPercent_lPercent(dtReal const percent) const
{
  return uPercent_u(u_l(percent * length()));
}

dtReal dtCurve::uPercent_l(dtReal const ll) const
{
  return uPercent_u(u_l(ll));
}

//
//
//
dtInt dtCurve::continuity(void) const { return -1; }

void dtCurve::dump(void)
{
  dt__info(
    dump(),
    << dt__eval(virtualClassName()) << std::endl
    << "uu = [ " << minPara(0) << ", " << maxPara(0) << "]" << std::endl
    << dt__eval(closed()) << std::endl
    << "dumpToString()" << std::endl
    << dumpToString()
  );
}

std::string dtCurve::dumpToString(void) const
{
  std::stringstream ss;

  ss << dt__dumpToString(<< "");

  return ss.str();
}

dt__C_addCloneForpVH(dtCurve);
} // namespace dtOO
