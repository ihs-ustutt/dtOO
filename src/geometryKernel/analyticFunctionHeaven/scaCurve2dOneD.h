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

#ifndef scaCurve2dOneD_H
#define scaCurve2dOneD_H

#include <dtOOTypeDef.h>

#include "scaOneD.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>

namespace dtOO {
class dtCurve2d;

class scaCurve2dOneD : public scaOneD {
public:
  dt__class(scaCurve2dOneD, analyticFunction);
  scaCurve2dOneD();
  scaCurve2dOneD(scaCurve2dOneD const &orig);
  scaCurve2dOneD(dtCurve2d const *const orig);
  scaCurve2dOneD *clone(void) const;
  scaCurve2dOneD *cloneTransformed(dtTransformer const *const dtT) const;
  scaCurve2dOneD *create(void) const;
  virtual ~scaCurve2dOneD();
  virtual dtReal YFloat(dtReal const &xx) const;
  dtCurve2d const *constPtrDtCurve2d(void) const;
  dtCurve2d *ptrDtCurve2d(void);
  void translate(dtVector2 const vector);
  std::vector<dtPoint2> getControlPoints(void) const;
  vectorHandling<renderInterface *> getRender(void) const;

private:
  ptrHandling<dtCurve2d> _dtC2d;
};
} // namespace dtOO
#endif /* scaCurve2dOneD_H */
