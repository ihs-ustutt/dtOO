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

#ifndef scaOneD_forceBSplineCurve2dAFXmlBuilder_H
#define scaOneD_forceBSplineCurve2dAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <xmlHeaven/aFXmlBuilder.h>

class QDomElement;

namespace dtOO {
class analyticFunction;
class constValue;
class baseContainer;

class scaOneD_forceBSplineCurve2dAFXmlBuilder : public aFXmlBuilder {
public:
  dt__classOnlyName(scaOneD_forceBSplineCurve2dAFXmlBuilder);
  dt__classSelfCreate(scaOneD_forceBSplineCurve2dAFXmlBuilder);
  scaOneD_forceBSplineCurve2dAFXmlBuilder();
  virtual ~scaOneD_forceBSplineCurve2dAFXmlBuilder();
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void buildPart(
    ::QDomElement const &toBuildP,
    baseContainer *const bC,
    lvH_constValue const *const cValP,
    lvH_analyticFunction const *const depSFunP,
    lvH_analyticFunction *sFunP
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* scaOneD_forceBSplineCurve2dAFXmlBuilder_H */
