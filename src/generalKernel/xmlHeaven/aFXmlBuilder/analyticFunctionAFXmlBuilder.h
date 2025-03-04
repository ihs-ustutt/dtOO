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

#ifndef analyticFunctionAFXmlBuilder_H
#define analyticFunctionAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <xmlHeaven/aFXmlBuilder.h>

class QDomElement;

namespace dtOO {
class scaFunction;
class constValue;
class baseContainer;

class analyticFunctionAFXmlBuilder : public aFXmlBuilder {
public:
  dt__classOnlyName(analyticFunctionAFXmlBuilder);
  dt__classSelfCreate(analyticFunctionAFXmlBuilder);
  analyticFunctionAFXmlBuilder();
  virtual ~analyticFunctionAFXmlBuilder();
  virtual void buildPart(
    ::QDomElement const &toBuild,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticFunction *result
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* analyticFunctionAFXmlBuilder_H */
