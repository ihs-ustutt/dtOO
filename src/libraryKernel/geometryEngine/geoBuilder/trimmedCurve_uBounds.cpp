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

#include "trimmedCurve_uBounds.h"

#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <Geom_TrimmedCurve.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
trimmedCurve_uBounds::trimmedCurve_uBounds(
  dtCurve const *dtC, dtReal const &min, dtReal const &max
)
{
  dt__ptrAss(dtOCCCurve const *occC, dtOCCCurve::ConstDownCast(dtC));

  Standard_Real U1 =
    calculationTypeHandling<Standard_Real, dtReal>::boundToRange(
      min, dtC->getUMin(), dtC->getUMax()
    );
  Standard_Real U2 =
    calculationTypeHandling<Standard_Real, dtReal>::boundToRange(
      max, dtC->getUMin(), dtC->getUMax()
    );
  Handle(Geom_TrimmedCurve) ts =
    new Geom_TrimmedCurve(occC->OCCRef().getOCC(), U1, U2);

  dtOCCCurveBase base;
  base.setOCC(ts);

  _dtC.reset(new dtOCCTrimmedCurve(base));
}

dtCurve *trimmedCurve_uBounds::result(void) { return _dtC->clone(); }

trimmedCurve_uBounds::~trimmedCurve_uBounds() {}
} // namespace dtOO
