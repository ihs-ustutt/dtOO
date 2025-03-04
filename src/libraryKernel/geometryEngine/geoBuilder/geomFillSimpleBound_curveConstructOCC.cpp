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

#include "geomFillSimpleBound_curveConstructOCC.h"

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCGeomFillBoundaryBase.h>
#include <geometryEngine/dtOCCGeomFillSimpleBound.h>
#include <logMe/logMe.h>

#include <GeomAdaptor_Curve.hxx>
#include <GeomFill_SimpleBound.hxx>
#include <Geom_Curve.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
geomFillSimpleBound_curveConstructOCC ::geomFillSimpleBound_curveConstructOCC(
  dtCurve const *const dtC
)
{
  dt__ptrAss(dtOCCCurve const *occC, dtOCCCurve::ConstDownCast(dtC));
  Handle(Geom_Curve) myParamCurve =
    Handle(Geom_Curve)::DownCast(occC->OCCRef().getOCC()->Copy());

  Handle(GeomAdaptor_Curve) ParamCurve = new GeomAdaptor_Curve(myParamCurve);

  Handle(GeomFill_SimpleBound) myBoundary = new GeomFill_SimpleBound(
    ParamCurve, Precision::Confusion(), Precision::Angular()
  );

  dt__tryOcc(_dtC.reset(new dtOCCGeomFillSimpleBound(
    dtOCCCurveBase(Handle(Geom_Curve)::DownCast(occC->OCCRef().getOCC()->Copy())
    ),
    dtOCCGeomFillBoundaryBase(myBoundary)
  ));
             , << "");
}

geomFillSimpleBound_curveConstructOCC ::~geomFillSimpleBound_curveConstructOCC()
{
}

dtCurve *geomFillSimpleBound_curveConstructOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
