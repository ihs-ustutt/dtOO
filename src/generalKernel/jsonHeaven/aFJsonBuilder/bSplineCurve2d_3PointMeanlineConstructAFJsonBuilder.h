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

#ifndef bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder_H
#define bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/vectorHandling.h>
#include <jsonHeaven/aFJsonBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class analyticFunction;
class constValue;
class baseContainer;

class bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder
  : public aFJsonBuilder {
public:
  dt__classOnlyName(bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder);
  bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder();
  virtual ~bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder();
  virtual lvH_analyticFunction buildPart(
    jsonPrimitive const &toBuildP,
    baseContainer const *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF
  ) const;
};
} // namespace dtOO
#endif /* bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder_H */
