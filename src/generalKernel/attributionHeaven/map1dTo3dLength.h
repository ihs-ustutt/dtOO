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

#ifndef map1dTo3dLength_H
#define map1dTo3dLength_H

#include <dtOOTypeDef.h>

#include "floatAtt.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
class map1dTo3d;

class map1dTo3dLength : public floatAtt {
public:
  dt__classOnlyName(map1dTo3dLength);
  map1dTo3dLength(map1dTo3d const *const m1d, dtReal const &ll);
  virtual ~map1dTo3dLength();
  map1dTo3dLength(map1dTo3dLength const &orig);
  virtual map1dTo3dLength *clone(void) const;
  virtual dtReal operator()(std::vector<dtReal> const &xx) const;
  //
  // get properties
  //
  virtual dtInt const &dimension() const;

private:
  map1dTo3d const *const _m1d;
  dtReal const _ll;
  dtInt const _dimension;
};
dt__H_addCloneForpVH(map1dTo3dLength);
} // namespace dtOO
#endif /* map1dTo3dLength_H */
