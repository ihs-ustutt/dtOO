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

#ifndef dtTransformerInvThreeD_H
#define dtTransformerInvThreeD_H

#include <dtOOTypeDef.h>

#include "dtTransformer.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
class dtTransformerInvThreeD : public dtTransformer {
public:
  dt__classOnlyName(dtTransformerInvThreeD);
  dtTransformerInvThreeD();
  virtual ~dtTransformerInvThreeD();
  dtTransformerInvThreeD(dtTransformerInvThreeD const &orig);
  dtTransformerInvThreeD(jsonPrimitive const &jE);
  //
  // overload
  //
  virtual dtTransformerInvThreeD *clone(void) const = 0;
  virtual dtTransformerInvThreeD *create(void) const = 0;
  virtual std::vector<dtPoint3> apply(std::vector<dtPoint3> const *const toTrans
  ) const = 0;
  virtual std::vector<dtVector3>
  apply(std::vector<dtVector3> const *const toTrans) const = 0;
  virtual std::vector<dtPoint3>
  retract(std::vector<dtPoint3> const *const toRetract) const = 0;
  virtual std::vector<dtVector3>
  retract(std::vector<dtVector3> const *const toRetract) const = 0;
  //
  //
  //
  using dtTransformer::apply;
  using dtTransformer::retract;
  std::vector<dtVector3> apply(std::vector<dtVector3> const &toTrans) const;
  std::vector<dtVector3> retract(std::vector<dtVector3> const &toRetract) const;
  dtVector3 apply(dtVector3 const &pp) const;
  dtVector3 retract(dtVector3 const &toRetract) const;
  std::vector<dtPoint3> apply(std::vector<dtPoint3> const &toTrans) const;
  std::vector<dtPoint3> retract(std::vector<dtPoint3> const &toRetract) const;
  dtPoint3 apply(dtPoint3 const &pp) const;
  dtPoint3 retract(dtPoint3 const &toRetract) const;
};
dt__H_addCloneForpVH(dtTransformerInvThreeD);
} // namespace dtOO

#endif /* dtTransformerInvThreeD_H */
