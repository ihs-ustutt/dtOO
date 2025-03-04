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

#ifndef bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder_H
#define bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>
#include <xmlHeaven/aFXmlBuilder.h>

class QDomElement;

namespace dtOO {
class analyticFunction;
class constValue;
class baseContainer;

class bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder : public aFXmlBuilder {
public:
  dt__classOnlyName(bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder);
  dt__classSelfCreate(bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder);
  bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder();
  virtual ~bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder();
  virtual std::vector<std::string> factoryAlias(void) const;
  virtual void buildPart(
    ::QDomElement const &toBuildP,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticFunction *result
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO
#endif /* bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder_H */
