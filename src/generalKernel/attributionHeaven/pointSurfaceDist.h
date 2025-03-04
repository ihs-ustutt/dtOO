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

#ifndef pointSurfaceDist_H
#define pointSurfaceDist_H

#include <dtOOTypeDef.h>

#include "floatAtt.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
class dtSurface;

class pointSurfaceDist : public floatAtt {
public:
  dt__classOnlyName(pointSurfaceDist);
  pointSurfaceDist(dtPoint3 const &p3, dtSurface const *const dtS);
  virtual ~pointSurfaceDist();
  pointSurfaceDist(pointSurfaceDist const &orig);
  virtual pointSurfaceDist *clone(void) const;
  virtual dtReal operator()(std::vector<dtReal> const &xx) const;
  //
  // get properties
  //
  virtual dtInt const &dimension() const;

private:
  dtPoint3 const _p3;
  dtInt const _dimension;
  dtSurface const *const _dtS;
};
dt__H_addCloneForpVH(pointSurfaceDist);
} // namespace dtOO
#endif /* pointSurfaceDist_H */
