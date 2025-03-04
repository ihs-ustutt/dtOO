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

#ifndef circle_radiusCoordinateSystemOCCAGXmlBuilder_H
#define circle_radiusCoordinateSystemOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <xmlHeaven/aGXmlBuilder.h>

namespace dtOO {
class analyticGeometry;
class analyticFunction;
class constValue;
class baseContainer;

class circle_radiusCoordinateSystemOCCAGXmlBuilder : public aGXmlBuilder {
public:
  dt__classOnlyName(circle_radiusCoordinateSystemOCCAGXmlBuilder);
  dt__classSelfCreate(circle_radiusCoordinateSystemOCCAGXmlBuilder);
  circle_radiusCoordinateSystemOCCAGXmlBuilder();
  virtual ~circle_radiusCoordinateSystemOCCAGXmlBuilder();
  virtual void buildPart(
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
#endif /* circle_radiusCoordinateSystemOCCAGXmlBuilder_H */
