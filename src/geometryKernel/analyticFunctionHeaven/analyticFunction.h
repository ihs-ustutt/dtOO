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

#ifndef analyticFunction_H
#define analyticFunction_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/renderInterface.h>
#include <logMe/dtMacros.h>
#include <vector>

#include "aFX.h"
#include "aFY.h"

#include <mainConceptFwd.h>

namespace dtOO {
class dtTransformer;

class analyticFunction : public labelHandling, public renderInterface {
public:
  dt__class(analyticFunction, labelHandling);
  analyticFunction();
  analyticFunction(const analyticFunction &orig);
  virtual ~analyticFunction();
  virtual analyticFunction *clone(void) const = 0;
  virtual analyticFunction *weakClone(void) const;
  virtual analyticFunction *cloneTransformed(dtTransformer const *const dtT
  ) const = 0;
  virtual analyticFunction *create(void) const = 0;
  virtual void dump(void) const;
  virtual std::string dumpToString(void) const;
  virtual aFY Y(aFX const &xx) const = 0;
  virtual dtInt xDim(void) const = 0;
  virtual dtInt yDim(void) const = 0;
  virtual dtReal xMin(dtInt const &dir) const = 0;
  virtual dtReal xMax(dtInt const &dir) const = 0;
  virtual aFX xMin(void) const;
  virtual aFX xMax(void) const;
  virtual aFX x_percent(aFX const &xx) const;
  virtual aFX percent_x(aFX const &xx) const;
  virtual aFX invY(aFY const &yy) const;
  virtual bool isCompound(void) const;
  virtual vectorHandling<analyticFunction *> const &vecRef(void) const;
  virtual std::map<int, dtInt> const &mapRef(void) const;
  virtual bool isTransformed(void) const;
  virtual std::pair<aFY, aFY> yBoundingBox(void) const;
  static aFX aFXZeroD(void);
  static aFX aFXOneD(dtReal const &x0);
  static aFX aFXTwoD(dtReal const &x0, dtReal const &x1);
  static aFX aFXThreeD(dtReal const &x0, dtReal const &x1, dtReal const &x2);
  static aFX aFXTwoD(dtPoint2 const &pp);
  static aFX aFXThreeD(dtPoint3 const &pp);
  static aFY aFYZeroD(void);
  static aFY aFYOneD(dtReal const &x0);
  static aFY aFYTwoD(dtReal const &x0, dtReal const &x1);
  static aFY aFYThreeD(dtReal const &x0, dtReal const &x1, dtReal const &x2);
  static aFY aFYTwoD(dtPoint2 const &pp);
  static aFY aFYThreeD(dtPoint3 const &pp);
  static aFY aFY_aFX(aFX const &xx);
  static aFX aFX_aFY(aFY const &yy);
  static dtReal distance(aFX const &x0, aFX const &x1);
  std::vector<dtReal> const operator()(dtReal const &xx) const;
  std::vector<dtReal> const
  operator()(dtReal const &xx, dtReal const &yy) const;
  std::vector<dtReal> const
  operator()(dtReal const &xx, dtReal const &yy, dtReal const &zz) const;
};
dt__H_addCloneForpVH(analyticFunction);
} // namespace dtOO
#endif /* analyticFunction_H */
