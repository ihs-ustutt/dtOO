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

#include "bVOInterface.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <logMe/logMe.h>

namespace dtOO {
bVOInterface::bVOInterface() { _bV = NULL; }

bVOInterface::~bVOInterface() {}

void bVOInterface::jInit(
  jsonPrimitive const &jE,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  _config = jE;
  dt__debug(jInit(), << _config.toStdString());
  optionHandling::jInit(_config);
  _bV = attachTo;
}

void bVOInterface::jInit(jsonPrimitive const &jE, boundedVolume *attachTo)
{
  jInit(jE, NULL, NULL, NULL, NULL, NULL, attachTo);
}

void bVOInterface::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  optionHandling::init(element, bC, cV, aF, aG);
  bVOInterface::jInit(jsonPrimitive(), bC, cV, aF, aG, bV, attachTo);
}

std::vector<std::string> bVOInterface::factoryAlias(void) const
{
  return std::vector<std::string>(0);
}

boundedVolume const &bVOInterface::constRefBoundedVolume(void) { return *_bV; }

boundedVolume *bVOInterface::ptrBoundedVolume(void) { return _bV; }

void bVOInterface::preUpdate(void) {}

void bVOInterface::postUpdate(void) {}

jsonPrimitive &bVOInterface::config(void) { return _config; }

jsonPrimitive const &bVOInterface::config(void) const { return _config; }
} // namespace dtOO
