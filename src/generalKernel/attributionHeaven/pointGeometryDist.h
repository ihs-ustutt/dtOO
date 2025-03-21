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

#ifndef pointGeometryDist_H
#define pointGeometryDist_H

#include <dtOOTypeDef.h>

#include "floatAtt.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
class analyticGeometry;

class pointGeometryDist : public floatAtt {
public:
  dt__classOnlyName(pointGeometryDist);
  pointGeometryDist(dtPoint3 const &p3, analyticGeometry const *const aG);
  virtual ~pointGeometryDist();
  pointGeometryDist(pointGeometryDist const &orig);
  virtual pointGeometryDist *clone(void) const;
  virtual bool outOfRange(::std::vector<dtReal> const &xx) const;
  virtual bool hasGrad(void) const;
  virtual dtReal operator()(std::vector<dtReal> const &xx) const;
  virtual ::std::vector<dtReal> grad(::std::vector<dtReal> const &xx) const;
  //
  // get properties
  //
  virtual dtInt const &dimension() const;

private:
  dtPoint3 const _p3;
  dtInt const _dimension;
  analyticGeometry const *const _aG;
};
dt__H_addCloneForpVH(pointGeometryDist);
} // namespace dtOO
#endif /* pointGeometryDist_H */
