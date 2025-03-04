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

#ifndef PICKLENGTHRANGE_H
#define PICKLENGTHRANGE_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <logMe/dtMacros.h>

namespace dtOO {
class analyticFunction;
class vec2dOneD;

class pickLengthRange : public dtTransformer {
public:
  dt__classOnlyName(pickLengthRange);
  pickLengthRange();
  pickLengthRange(const pickLengthRange &orig);
  virtual ~pickLengthRange();
  virtual dtTransformer *clone(void) const;
  virtual dtTransformer *create(void) const;
  virtual bool isNecessary(void) const;
  void init(
    ::QDomElement const *tE,
    baseContainer *const bC,
    lvH_constValue const *const cV,
    lvH_analyticFunction const *const aF,
    lvH_analyticGeometry const *const aG
  );
  virtual lvH_analyticGeometry apply(lvH_analyticGeometry const *const aGeoVecP
  ) const;

private:
  dtReal _lengthOneStart;
  dtReal _lengthTwoStart;
  dtReal _lengthOneEnd;
  dtReal _lengthTwoEnd;
  bool _createAnalyticSurface;
  bool _createSplineCurve3d;
  static bool _registrated;
};
} // namespace dtOO

#endif /* PICKLENGTHRANGE_H */
