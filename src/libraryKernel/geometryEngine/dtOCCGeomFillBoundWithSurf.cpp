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

#include "dtOCCGeomFillBoundWithSurf.h"

#include "dtOCCCurveBase.h"
#include "dtOCCGeomFillBoundaryBase.h"
#include <interfaceHeaven/calculationTypeHandling.h>
#include <logMe/logMe.h>

#include <GeomFill_BoundWithSurf.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
dtOCCGeomFillBoundWithSurf::dtOCCGeomFillBoundWithSurf()
  : dtOCCGeomFillBoundary()
{
  _ptr = NULL;
}

dtOCCGeomFillBoundWithSurf::dtOCCGeomFillBoundWithSurf(
  dtOCCCurveBase const &cBase, dtOCCGeomFillBoundaryBase const &bBase
)
  : dtOCCGeomFillBoundary(cBase, bBase)
{
  dt__mustCast(
    OCCBoundaryRef().getOCC().get(), GeomFill_BoundWithSurf const, _ptr
  );
}

dtOCCGeomFillBoundWithSurf::~dtOCCGeomFillBoundWithSurf() {}

dtOCCGeomFillBoundWithSurf *dtOCCGeomFillBoundWithSurf::clone(void) const
{
  return new dtOCCGeomFillBoundWithSurf(OCCRef(), OCCBoundaryRef());
}

dtReal dtOCCGeomFillBoundWithSurf::minPara(dtInt const &dir) const
{
  dt__throwIf(dir != 0, minPara());
  Standard_Real U0;
  Standard_Real U1;
  dt__tryOcc(_ptr->Bounds(U0, U1);, << " U = [ " << U0 << ", " << U1 << " ]");
  return floatHandling::nextIfSmaller(U0);
}

dtReal dtOCCGeomFillBoundWithSurf::maxPara(dtInt const &dir) const
{
  dt__throwIf(dir != 0, maxPara());
  Standard_Real U0;
  Standard_Real U1;
  dt__tryOcc(_ptr->Bounds(U0, U1);, << " U = [ " << U0 << ", " << U1 << " ]");
  return floatHandling::nextIfSmaller(U1);
}

dtPoint3 dtOCCGeomFillBoundWithSurf::point(dtReal const uu) const
{
  Standard_Real uR = static_cast<Standard_Real>(uu);
  gp_Pnt pp;
  dt__tryOcc(pp = _ptr->Value(uR);, << dt__eval(uu));

  return dtPoint3(
    static_cast<dtReal>(pp.Coord(1)),
    static_cast<dtReal>(pp.Coord(2)),
    static_cast<dtReal>(pp.Coord(3))
  );
}
} // namespace dtOO
