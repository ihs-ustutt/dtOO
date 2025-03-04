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

#ifndef SCAFUNCTION_H
#define SCAFUNCTION_H

#include <dtOOTypeDef.h>

#include "analyticFunction.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class scaFunction : public analyticFunction {
public:
  dt__class(scaFunction, analyticFunction);
  scaFunction();
  scaFunction(scaFunction const &orig);
  virtual scaFunction *clone(void) const = 0;
  virtual ~scaFunction();
  virtual dtInt yDim(void) const;
  virtual dtReal YFloat(aFX const &xx) const = 0;
  virtual aFY Y(aFX const &xx) const;
};
dt__H_addCloneForpVH(scaFunction);
} // namespace dtOO
#endif /* SCAFUNCTION_H */
