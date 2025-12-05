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

#ifndef gslMinFloatAttr_H
#define gslMinFloatAttr_H

#include "minFloatAttr.h"
#include <dtOOTypeDef.h>

#include <attributionHeaven/floatAtt.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class gslMinFloatAttr : public minFloatAttr {
public:
  dt__class(gslMinFloatAttr, minFloatAttr);
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtReal> const &guess,
    std::vector<dtReal> const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  )
    : minFloatAttr(attribute, guess, step, precision, maxIterations) {};
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtPoint2 const &guess,
    dtPoint2 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  )
    : minFloatAttr(attribute, guess, step, precision, maxIterations) {};
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtPoint2> const &guess,
    dtPoint2 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  )
    : minFloatAttr(attribute, guess, step, precision, maxIterations) {};
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtPoint3 const &guess,
    dtPoint3 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  )
    : minFloatAttr(attribute, guess, step, precision, maxIterations) {};
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    std::vector<dtPoint3> const &guess,
    dtPoint3 const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  )
    : minFloatAttr(attribute, guess, step, precision, maxIterations) {};
  gslMinFloatAttr(
    dt__pH(floatAtt) const &attribute,
    dtReal const &guess,
    dtReal const &step,
    dtReal const &precision,
    dtInt const &maxIterations = 100
  )
    : minFloatAttr(attribute, guess, step, precision, maxIterations) {};
  gslMinFloatAttr(minFloatAttr const &orig) : minFloatAttr(orig) {};
  virtual ~gslMinFloatAttr();
  virtual gslMinFloatAttr *clone(void) const;
  virtual bool perform();

private:
};
dt__H_addCloneForpVH(gslMinFloatAttr);
} // namespace dtOO
#endif /* gslMinFloatAttr_H */
