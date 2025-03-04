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

#include "analyticFunctionAFXmlBuilder.h"

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <dtLinearAlgebra.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool analyticFunctionAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(analyticFunctionAFXmlBuilder, new analyticFunctionAFXmlBuilder())
  );

analyticFunctionAFXmlBuilder::analyticFunctionAFXmlBuilder() {}

analyticFunctionAFXmlBuilder::~analyticFunctionAFXmlBuilder() {}

void analyticFunctionAFXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  //
  // check input
  //
  dt__throwIf(
    !dtXmlParserBase::hasChild("analyticFunction", toBuild), buildPart()
  );

  //
  // copy
  //
  ::QDomElement wElement =
    dtXmlParserBase::getChild("analyticFunction", toBuild);
  lvH_analyticFunction toCopy;
  dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, &toCopy);
  dt__forAllIndex(toCopy, ii) result->push_back(toCopy[ii]);
}
} // namespace dtOO
