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

#ifndef DTOCCLINE2D_H
#define DTOCCLINE2D_H

#include <dtOOTypeDef.h>

#include "dtOCCCurve2d.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

class Geom2d_Line;

namespace dtOO {
class dtOCCCurve2dBase;

class dtOCCLine2d : public dtOCCCurve2d {
public:
  dt__class(dtOCCLine2d, dtCurve2d);
  dtOCCLine2d();
  dtOCCLine2d(dtOCCCurve2dBase const &orig);
  virtual ~dtOCCLine2d();
  virtual dtOCCLine2d *clone(void) const;
  virtual dtInt order(void) const;
  virtual dtInt nControlPoints(void) const;
  virtual dtPoint2 controlPoint(dtInt const nPoint) const;

private:
  Geom2d_Line const *_ptr;
};
} // namespace dtOO

#endif /* DTOCCLINE2D_H */
