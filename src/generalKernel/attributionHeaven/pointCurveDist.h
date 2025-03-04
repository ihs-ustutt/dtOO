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

#ifndef pointCurveDist_H
#define pointCurveDist_H

#include <dtOOTypeDef.h>

#include "floatAtt.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
class dtCurve;

class pointCurveDist : public floatAtt {
public:
  dt__classOnlyName(pointCurveDist);
  pointCurveDist(dtPoint3 const &p3, dtCurve const *const dtC);
  virtual ~pointCurveDist();
  pointCurveDist(pointCurveDist const &orig);
  virtual pointCurveDist *clone(void) const;
  virtual dtReal operator()(std::vector<dtReal> const &xx) const;
  //
  // get properties
  //
  virtual dtInt const &dimension() const;

private:
  dtPoint3 const _p3;
  dtInt const _dimension;
  dtCurve const *const _dtC;
};
dt__H_addCloneForpVH(pointCurveDist);
} // namespace dtOO
#endif /* pointCurveDist_H */
