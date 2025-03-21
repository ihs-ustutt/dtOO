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

#ifndef SCAONEDINMAP1DTO3D_H
#define SCAONEDINMAP1DTO3D_H

#include <dtOOTypeDef.h>

#include "map1dTo3d.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class scaOneD;

class scaOneDInMap1dTo3d : public map1dTo3d {
public:
  dt__class(scaOneDInMap1dTo3d, analyticGeometry);
  scaOneDInMap1dTo3d();
  scaOneDInMap1dTo3d(const scaOneDInMap1dTo3d &orig);
  scaOneDInMap1dTo3d(scaOneD const *const s1d, map1dTo3d const *const m1d);
  scaOneDInMap1dTo3d(
    scaOneD const *const s1d, map1dTo3d const *const m1d, bool const &percentF
  );
  virtual ~scaOneDInMap1dTo3d();
  //
  // overload
  //
  virtual scaOneDInMap1dTo3d *clone(void) const;
  virtual scaOneDInMap1dTo3d *cloneTransformed(dtTransformer const *const dtT
  ) const;
  virtual scaOneDInMap1dTo3d *create(void) const;
  virtual dtPoint3 getPoint(dtReal const &uu) const;
  virtual bool isClosed(dtInt const &dir) const;
  virtual dtReal getMin(dtInt const &dir) const;
  virtual dtReal getMax(dtInt const &dir) const;
  //
  //
  //
  scaOneD const *ptrToScaOneD(void) const;
  map1dTo3d const *ptrToMap1dTo3d(void) const;

private:
  dt__pH(scaOneD) _s1d;
  dt__pH(map1dTo3d) _m1d;
  bool _percentF;
};
} // namespace dtOO
#endif /* SCAONEDINMAP1DTO3D_H */
