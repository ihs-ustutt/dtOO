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

#ifndef floatAtt_H
#define floatAtt_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <vector>

namespace dtOO {
class analyticGeometry;
class dtCurve;

class floatAtt {
public:
  dt__classOnlyName(floatAtt);
  floatAtt();
  virtual ~floatAtt();
  floatAtt(floatAtt const &orig);
  virtual floatAtt *clone(void) const = 0;
  virtual dtInt const &dimension() const = 0;
  virtual bool outOfRange(::std::vector<dtReal> const &xx) const;
  virtual dtReal outOfRangeResult() const;
  dtReal rangeCheckAndCall(::std::vector<dtReal> const &xx) const;
  virtual bool hasGrad(void) const;
  //
  // strategy overload
  //
  virtual dtReal operator()(::std::vector<dtReal> const &xx) const = 0;
  virtual ::std::vector<dtReal> grad(::std::vector<dtReal> const &xx) const;
};
dt__H_addCloneForpVH(floatAtt);
} // namespace dtOO
#endif /* floatAtt_H */
