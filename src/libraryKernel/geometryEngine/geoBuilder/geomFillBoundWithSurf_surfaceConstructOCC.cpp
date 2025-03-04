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

#include "geomFillBoundWithSurf_surfaceConstructOCC.h"

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCGeomFillBoundWithSurf.h>
#include <geometryEngine/dtOCCGeomFillBoundary.h>
#include <geometryEngine/dtOCCGeomFillBoundaryBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineCurve_approxGeomCurve2dInGeomSurface.h>
#include <logMe/logMe.h>

#include <Adaptor3d_CurveOnSurface.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <Geom2d_Curve.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomFill_BoundWithSurf.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Surface.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
geomFillBoundWithSurf_surfaceConstructOCC ::
  geomFillBoundWithSurf_surfaceConstructOCC(
    dtSurface const *const dtS, dtCurve2d const *const dtC2d
  )
{
  dt__ptrAss(dtOCCSurface const *occS, dtOCCSurface::ConstDownCast(dtS));
  Handle(Geom_Surface) mySurface =
    Handle(Geom_Surface)::DownCast(occS->OCCRef().getOCC()->Copy());
  dt__ptrAss(dtOCCCurve2d const *occC2d, dtOCCCurve2d::ConstDownCast(dtC2d));
  Handle(Geom2d_Curve) myParamCurve =
    Handle(Geom2d_Curve)::DownCast(occC2d->OCCRef().getOCC()->Copy());

  Handle(GeomAdaptor_Surface) Surface = new GeomAdaptor_Surface(mySurface);
  Handle(Geom2dAdaptor_Curve) ParamCurve =
    new Geom2dAdaptor_Curve(myParamCurve);
  Adaptor3d_CurveOnSurface CurveOnSurf(ParamCurve, Surface);

  Handle(GeomFill_BoundWithSurf) myBoundary = new GeomFill_BoundWithSurf(
    CurveOnSurf, Precision::Confusion(), Precision::Angular()
  );

  ptrHandling<dtCurve> bsC(
    bSplineCurve_approxGeomCurve2dInGeomSurface(dtC2d, dtS).result()
  );
  Handle(Geom_Curve) bsCurve = Handle(Geom_Curve
  )::DownCast(dtOCCBSplineCurve::DownCast(bsC.get())->OCCRef().getOCC()->Copy()
  );
  dtOCCCurveBase baseC;
  baseC.setOCC(bsCurve);
  dtOCCGeomFillBoundaryBase baseB;
  baseB.setOCC(myBoundary);
  _dtC.reset(new dtOCCGeomFillBoundWithSurf(baseC, baseB));
}

geomFillBoundWithSurf_surfaceConstructOCC ::
  ~geomFillBoundWithSurf_surfaceConstructOCC()
{
}

dtCurve *geomFillBoundWithSurf_surfaceConstructOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
