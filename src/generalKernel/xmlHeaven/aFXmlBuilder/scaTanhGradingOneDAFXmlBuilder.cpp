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

#include "scaTanhGradingOneDAFXmlBuilder.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaTanhGradingOneD.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>
#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include <QtXml/QDomElement>

namespace dtOO {
bool scaTanhGradingOneDAFXmlBuilder::_registrated =
  aFXmlBuilderFactory::registrate(dt__tmpPtr(
    scaTanhGradingOneDAFXmlBuilder, new scaTanhGradingOneDAFXmlBuilder()
  ));

scaTanhGradingOneDAFXmlBuilder::scaTanhGradingOneDAFXmlBuilder() {}

scaTanhGradingOneDAFXmlBuilder::~scaTanhGradingOneDAFXmlBuilder() {}

void scaTanhGradingOneDAFXmlBuilder::buildPart(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  result->push_back(new scaTanhGradingOneD(
    dtXmlParserBase::getAttributeFloatVectorMuParse("c", toBuild, cV, aF),
    dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF),
    dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF),
    dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF)
  ));
}

void scaTanhGradingOneDAFXmlBuilder::buildPartCompound(
  ::QDomElement const &toBuild,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticFunction *result
) const
{
  result->push_back(new scaTanhGradingOneDCompound(scaTanhGradingOneD(
    dtXmlParserBase::getAttributeFloatVectorMuParse("c", toBuild, cV, aF),
    dtXmlParserBase::getAttributeFloatMuParse("g", toBuild, cV, aF),
    dtXmlParserBase::getAttributeFloatMuParse("gMin", toBuild, cV, aF),
    dtXmlParserBase::getAttributeFloatMuParse("gMax", toBuild, cV, aF)
  )));
}
} // namespace dtOO
