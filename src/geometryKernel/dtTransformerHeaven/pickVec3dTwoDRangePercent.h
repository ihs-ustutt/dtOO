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

#ifndef PICKVEC3DTWODRANGEPERCENT_H
#define PICKVEC3DTWODRANGEPERCENT_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class analyticFunction;

class pickVec3dTwoDRangePercent : public dtTransformer {
public:
  dt__classOnlyName(pickVec3dTwoDRangePercent);
  pickVec3dTwoDRangePercent();
  pickVec3dTwoDRangePercent(const pickVec3dTwoDRangePercent &orig);
  pickVec3dTwoDRangePercent(jsonPrimitive const &jE);
  virtual ~pickVec3dTwoDRangePercent();
  virtual dtTransformer *clone(void) const;
  virtual dtTransformer *create(void) const;
  virtual bool isNecessary(void) const;
  virtual void jInit(
    jsonPrimitive const &jE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  void init(
    ::QDomElement const *tE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  virtual lvH_analyticFunction apply(lvH_analyticFunction const *const aFVecP
  ) const;

private:
  static bool _registrated;
};
} // namespace dtOO

#endif /* PICKVEC3DTWODRANGEPERCENT_H */
