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

#include "doNothing.h"
#include <logMe/logMe.h>

#include "dtTransformerFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>

namespace dtOO {
bool doNothing::_registrated =
  dtTransformerFactory::registrate(dt__tmpPtr(doNothing, new doNothing()));

doNothing::doNothing() : dtTransformer() {}

doNothing::~doNothing() {}

dtTransformer *doNothing::clone(void) const
{
  dt__throw(clone(), << "Not yet implemented.");
}

dtTransformer *doNothing::create(void) const { return new doNothing(); }

std::vector<dtPoint2 *>
doNothing::apply(std::vector<dtPoint2 *> const *const pointVecP) const
{
  std::vector<dtPoint2 *> ret;
  dt__forAllRefAuto(*pointVecP, aPoint) ret.push_back(aPoint);
  return ret;
}

std::vector<dtPoint3 *>
doNothing::apply(std::vector<dtPoint3 *> const *const pointVecP) const
{
  std::vector<dtPoint3 *> ret;
  dt__forAllRefAuto(*pointVecP, aPoint) ret.push_back(aPoint);
  return ret;
}

lvH_analyticGeometry doNothing::apply(lvH_analyticGeometry const *const aGeoVecP
) const
{
  lvH_analyticGeometry ret;
  dt__forAllRefAuto(*aGeoVecP, aGeo) ret.push_back(aGeo->clone());
  return ret;
}

lvH_analyticFunction doNothing::apply(lvH_analyticFunction const *const sFunVecP
) const
{
  lvH_analyticFunction ret;
  dt__forAllRefAuto(*sFunVecP, aFun) ret.push_back(aFun->clone());
  return ret;
}

void doNothing::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cValP,
  lvH_analyticFunction const *const sFunP,
  lvH_analyticGeometry const *const depAGeoP
)
{
}

bool doNothing::isNecessary(void) const { return false; }
} // namespace dtOO
