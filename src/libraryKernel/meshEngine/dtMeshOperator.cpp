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

#include "dtMeshOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
dtMeshOperator::dtMeshOperator() {}

dtMeshOperator::~dtMeshOperator() {}

std::vector<std::string> dtMeshOperator::factoryAlias(void) const
{
  return std::vector<std::string>(0);
}

void dtMeshOperator::jInit(
  jsonPrimitive const &jE,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  _config = jE;
  dt__debug(jInit(), << _config.toStdString());
  labelHandling::jInit(_config);
  optionHandling::jInit(_config);
}

void dtMeshOperator::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  labelHandling::setLabel(qtXmlPrimitive::getAttributeStr("label", element));
  optionHandling::init(element, bC, cV, aF, aG);
}

jsonPrimitive &dtMeshOperator::config(void) { return _config; }

jsonPrimitive const &dtMeshOperator::config(void) const { return _config; }
} // namespace dtOO
