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

#ifndef bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H
#define bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <xmlHeaven/aFXmlBuilder.h>

namespace dtOO {
class analyticFunction;
class constValue;
class baseContainer;

class bSplineCurve_curveConnectConstructOCCAFXmlBuilder : public aFXmlBuilder {
public:
  dt__classOnlyName(bSplineCurve_curveConnectConstructOCCAFXmlBuilder);
  dt__classSelfCreate(bSplineCurve_curveConnectConstructOCCAFXmlBuilder);
  bSplineCurve_curveConnectConstructOCCAFXmlBuilder();
  virtual ~bSplineCurve_curveConnectConstructOCCAFXmlBuilder();
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
#endif /* bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H */
