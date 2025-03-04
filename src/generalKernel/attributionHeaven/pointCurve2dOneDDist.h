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

#ifndef pointCurve2dOneDDist_H
#define pointCurve2dOneDDist_H

#include <dtOOTypeDef.h>

#include "floatAtt.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
class dtCurve2d;

class pointCurve2dOneDDist : public floatAtt {
public:
  dt__classOnlyName(pointCurve2dOneDDist);
  pointCurve2dOneDDist(
    dtReal const &value, dtCurve2d const *const dtC, dtInt const &dir
  );
  virtual ~pointCurve2dOneDDist();
  pointCurve2dOneDDist(pointCurve2dOneDDist const &orig);
  virtual pointCurve2dOneDDist *clone(void) const;
  virtual dtReal operator()(std::vector<dtReal> const &xx) const;
  //
  // get properties
  //
  virtual dtInt const &dimension() const;

private:
  dtReal const _value;
  dtInt const _dir;
  dtInt const _dimension;
  dtCurve2d const *const _dtC;
};
dt__H_addCloneForpVH(pointCurve2dOneDDist);
} // namespace dtOO
#endif /* pointCurve2dOneDDist_H */
