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

#ifndef aFXmlBuilder_H
#define aFXmlBuilder_H

#include <dtOOTypeDef.h>

#include "aFXmlBuilder.h"
#include "dtXmlParserBase.h"
#include <interfaceHeaven/labeledVectorHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class baseContainer;
class constValue;
class analyticFunction;

class aFXmlBuilder {
public:
  dt__classOnlyName(aFXmlBuilder);
  aFXmlBuilder();
  ~aFXmlBuilder();
  virtual aFXmlBuilder *create(void) const = 0;
  virtual void buildPart(
    ::QDomElement const &toBuild,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticFunction *result
  ) const = 0;
  virtual void buildPartCompound(
    ::QDomElement const &toBuild,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticFunction *result
  ) const;
  virtual std::vector<std::string> factoryAlias(void) const;
};
} // namespace dtOO
#endif /* aFXmlBuilder_H */
