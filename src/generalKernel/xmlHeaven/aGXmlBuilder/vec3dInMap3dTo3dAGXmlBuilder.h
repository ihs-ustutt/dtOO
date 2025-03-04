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

#ifndef vec3dInMap3dTo3dAGXmlBuilder_H
#define vec3dInMap3dTo3dAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <xmlHeaven/aGXmlBuilder.h>

class QDomElement;

namespace dtOO {
class analyticGeometry;
class analyticFunction;
class constValue;
class baseContainer;

class vec3dInMap3dTo3dAGXmlBuilder : public aGXmlBuilder {
public:
  dt__classOnlyName(vec3dInMap3dTo3dAGXmlBuilder);
  dt__classSelfCreate(vec3dInMap3dTo3dAGXmlBuilder);
  vec3dInMap3dTo3dAGXmlBuilder();
  virtual ~vec3dInMap3dTo3dAGXmlBuilder();
  virtual void buildPart(
    ::QDomElement const &toBuild,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_analyticGeometry *result
  ) const;
  virtual void buildPartCompound(
    ::QDomElement const &toBuild,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG,
    lvH_analyticGeometry *result
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* vec3dInMap3dTo3dAGXmlBuilder_H */
