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

#ifndef curveCurveDist_H
#define curveCurveDist_H

#include <dtOOTypeDef.h>

#include "floatAtt.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
class dtCurve;

class curveCurveDist : public floatAtt {
public:
  dt__classOnlyName(curveCurveDist);
  curveCurveDist(dtCurve const *const c0, dtCurve const *const c1);
  virtual ~curveCurveDist();
  curveCurveDist(curveCurveDist const &orig);
  virtual curveCurveDist *clone(void) const;
  virtual dtReal operator()(std::vector<dtReal> const &xx) const;
  //
  // get properties
  //
  virtual dtInt const &dimension() const;

private:
  dtCurve const *const _c0;
  dtCurve const *const _c1;
  dtInt const _dimension;
};
dt__H_addCloneForpVH(curveCurveDist);
} // namespace dtOO
#endif /* curveCurveDist_H */
