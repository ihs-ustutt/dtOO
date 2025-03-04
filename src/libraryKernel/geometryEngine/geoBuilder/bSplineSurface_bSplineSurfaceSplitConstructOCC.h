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

#ifndef bSplineSurface_bSplineSurfaceSplitConstructOCC_H
#define bSplineSurface_bSplineSurfaceSplitConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtSurface;

class bSplineSurface_bSplineSurfaceSplitConstructOCC {
public:
  dt__classOnlyName(bSplineSurface_bSplineSurfaceSplitConstructOCC);
  bSplineSurface_bSplineSurfaceSplitConstructOCC(
    dtSurface const *const dtS,
    dtInt const &dir,
    dtReal const &p0,
    dtReal const &p1
  );
  virtual ~bSplineSurface_bSplineSurfaceSplitConstructOCC();
  dtSurface *result(void);

private:
  dt__pH(dtSurface) _dtS;
};
} // namespace dtOO
#endif /* bSplineSurface_bSplineSurfaceSplitConstructOCC_H */
