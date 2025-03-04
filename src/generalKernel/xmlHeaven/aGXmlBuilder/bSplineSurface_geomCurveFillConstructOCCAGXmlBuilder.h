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

#ifndef bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H
#define bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <xmlHeaven/aGXmlBuilder.h>

namespace dtOO {
class analyticGeometry;
class analyticFunction;
class constValue;
class baseContainer;

class bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder
  : public aGXmlBuilder {
public:
  dt__classOnlyName(bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder);
  dt__classSelfCreate(bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder);
  bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder();
  virtual ~bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder();
  virtual std::vector<std::string> factoryAlias(void) const;
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

#endif /* bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder_H */
