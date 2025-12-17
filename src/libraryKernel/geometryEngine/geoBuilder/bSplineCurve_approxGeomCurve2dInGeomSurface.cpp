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

#include "bSplineCurve_approxGeomCurve2dInGeomSurface.h"

#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <logMe/logMe.h>

#include <Adaptor2d_Curve2d.hxx>
#include <Approx_CurveOnSurface.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <Geom2d_Curve.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Surface.hxx>
#include <Precision.hxx>
#include <Standard_Failure.hxx>

namespace dtOO {
bSplineCurve_approxGeomCurve2dInGeomSurface ::
  bSplineCurve_approxGeomCurve2dInGeomSurface(
    dtCurve2d const *const dtC2d, dtSurface const *const dtS
  )
{
  Handle(Geom2d_Curve) gc = Handle(Geom2d_Curve
  )::DownCast(dtOCCCurve2d::ConstDownCast(dtC2d)->OCCRef().getOCC()->Copy());

  Handle(Geom_Surface) gs = Handle(Geom_Surface
  )::DownCast(dtOCCSurface::ConstDownCast(dtS)->OCCRef().getOCC()->Copy());

  Handle(Adaptor2d_Curve2d) hc = new Geom2dAdaptor_Curve(gc);
  Handle(Adaptor3d_Surface) hs = new GeomAdaptor_Surface(gs);

  Approx_CurveOnSurface acos(
    hc, hs, Standard_Real(0.0), Standard_Real(1.0), Precision::Confusion()
  );
  acos.Perform(
    Standard_Integer(5), Standard_Integer(2), GeomAbs_Shape::GeomAbs_C2
  );
  Handle(Geom_BSplineCurve) curve = acos.Curve3d();

  dtOCCCurveBase base;
  base.setOCC(curve);
  _dtC.reset(new dtOCCBSplineCurve(base));
}

bSplineCurve_approxGeomCurve2dInGeomSurface ::
  ~bSplineCurve_approxGeomCurve2dInGeomSurface()
{
}

dtCurve *bSplineCurve_approxGeomCurve2dInGeomSurface::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
