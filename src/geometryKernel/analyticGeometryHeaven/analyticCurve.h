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

#ifndef analyticCurve_H
#define analyticCurve_H

#include <dtOOTypeDef.h>

#include "map1dTo3d.h"
#include <deprecatedMacro.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
class dtCurve;

class analyticCurve : public map1dTo3d {
public:
  dt__class(analyticCurve, analyticGeometry);
  analyticCurve();
  analyticCurve(analyticCurve const &orig);
  analyticCurve(dtCurve const &orig);
  analyticCurve(dtCurve const *const orig);
  virtual ~analyticCurve();
  //
  // overload
  //
  virtual analyticCurve *create(void) const;
  virtual analyticCurve *clone(void) const;
  virtual analyticCurve *cloneTransformed(dtTransformer const *const dtT) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  virtual dtPoint3 getPoint(dtReal const &uu) const;
  //
  // optional overload
  //
  virtual dtVector3 firstDerU(dtReal const uu) const;
  virtual map1dTo3d *segment(dtReal const &u0, dtReal const &u1) const;
  virtual dtReal l_u(dtReal const &uu) const;
  virtual dtReal u_l(dtReal const &ll) const;
  virtual vectorHandling<renderInterface *> getExtRender(void) const;
  dtCurve *ptrDtCurve(void) const;
  dtCurve const *const ptrConstDtCurve(void) const;
  std::string dumpToString(void) const;

private:
  dt__pH(dtCurve) _dtC;
};
} // namespace dtOO
#endif /* analyticCurve_H */
