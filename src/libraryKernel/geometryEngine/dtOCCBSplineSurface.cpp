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

#include "dtOCCBSplineSurface.h"

#include "dtOCCBSplineCurve.h"
#include "dtOCCCurveBase.h"
#include "dtOCCSurfaceBase.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {
dtOCCBSplineSurface::dtOCCBSplineSurface() : dtOCCSurface() { _ptr = NULL; }

dtOCCBSplineSurface::dtOCCBSplineSurface(const dtOCCSurfaceBase &orig)
  : dtOCCSurface(orig)
{
  dt__mustCast(OCCRef().getOCC().get(), Geom_BSplineSurface const, _ptr);
}

dtOCCBSplineSurface::~dtOCCBSplineSurface() {}

dtOCCBSplineSurface *dtOCCBSplineSurface::clone(void) const
{
  return new dtOCCBSplineSurface(OCCRef());
}

dtPoint3 dtOCCBSplineSurface::controlPoint(dtInt const uI, dtInt const vI) const
{
  Standard_Integer uSi = static_cast<Standard_Integer>(uI + 1);
  Standard_Integer vSi = static_cast<Standard_Integer>(vI + 1);
  gp_Pnt pp = _ptr->Pole(uSi, vSi);

  return dtPoint3(
    static_cast<dtReal>(pp.Coord(1)),
    static_cast<dtReal>(pp.Coord(2)),
    static_cast<dtReal>(pp.Coord(3))
  );
}

void dtOCCBSplineSurface::setControlPoint(
  dtInt const uI, dtInt const vI, dtPoint3 const point
)
{
  Standard_Integer uSi = static_cast<Standard_Integer>(uI + 1);
  Standard_Integer vSi = static_cast<Standard_Integer>(vI + 1);

  gp_Pnt pp(point.x(), point.y(), point.z());

  Handle(Geom_Surface) occS = dtOCCSurface::OCCRef().getOCC();
  Handle(Geom_BSplineSurface) occBss =
    Handle(Geom_BSplineSurface)::DownCast(occS);

  occBss->SetPole(uSi, vSi, pp);
}

int dtOCCBSplineSurface::nControlPoints(dtInt const dim) const
{
  switch (dim)
  {
  case 0:
    return static_cast<int>(_ptr->NbUPoles());
  case 1:
    return static_cast<int>(_ptr->NbVPoles());
  default:
    dt__throw(
      nControlPoints(),
      << dt__eval(dim) << std::endl
      << "dim should be 0 or 1."
    );
  }
}

dtCurve *dtOCCBSplineSurface::segmentConstU(
  dtReal const uu, dtReal const vvMin, dtReal const vvMax
) const
{
  Standard_Real uR = static_cast<Standard_Real>(uu);
  Handle(Geom_Curve) cc = _ptr->UIso(uR);

  Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);

  Standard_Real v1R = static_cast<Standard_Real>(vvMin);
  Standard_Real v2R = static_cast<Standard_Real>(vvMax);

  dt__solution(v1R < ccB->FirstParameter(),
               v1R = ccB->FirstParameter(); //,
                                            //			segmentConstU(),
               //			<< "v1R = ccB->FirstParameter();"
  );
  dt__solution(v2R > ccB->LastParameter(),
               v2R = ccB->LastParameter(); //,
                                           //			segmentConstU(),
                                           //			<< "v2R = ccB->LastParameter();"
  );

  dt__tryOcc(ccB->Segment(v1R, v2R);,
                                    << dt__eval(v1R) << std::endl
                                    << dt__eval(v2R));

  dtOCCCurveBase base;
  base.setOCC(ccB);

  return new dtOCCBSplineCurve(base);
}

dtCurve *dtOCCBSplineSurface::segmentConstV(
  dtReal const vv, dtReal const uuMin, dtReal const uuMax
) const
{
  Standard_Real vR = static_cast<Standard_Real>(vv);
  Handle(Geom_Curve) cc = _ptr->VIso(vR);

  Handle(Geom_BSplineCurve) ccB = Handle(Geom_BSplineCurve)::DownCast(cc);

  Standard_Real u1R = static_cast<Standard_Real>(uuMin);
  Standard_Real u2R = static_cast<Standard_Real>(uuMax);

  dt__solution(u1R < ccB->FirstParameter(),
               u1R = ccB->FirstParameter(); //,
                                            //			segmentConstV(),
               //			<< "u1R = ccB->FirstParameter();"
  );
  dt__solution(u2R > ccB->LastParameter(),
               u2R = ccB->LastParameter(); //,
                                           //			segmentConstV(),
                                           //			<< "u2R = ccB->LastParameter();"
  );

  dt__tryOcc(ccB->Segment(u1R, u2R);,
                                    << dt__eval(u1R) << std::endl
                                    << dt__eval(u2R));

  dtOCCCurveBase base;
  base.setOCC(ccB);

  return new dtOCCBSplineCurve(base);
}

std::string dtOCCBSplineSurface::dumpToString(void) const
{
  Standard_Real U1;
  Standard_Real U2;
  Standard_Real V1;
  Standard_Real V2;

  _ptr->Bounds(U1, U2, V1, V2);

  std::stringstream ss;

  ss << dt__dumpToString(
    << dtOCCSurface::dumpToString() << std::endl
    << dt__eval(U1) << std::endl
    << dt__eval(U2) << std::endl
    << dt__eval(V1) << std::endl
    << dt__eval(V2) << std::endl
    << dt__eval(_ptr->IsURational()) << std::endl
    << dt__eval(_ptr->IsVRational()) << std::endl
    << dt__eval(_ptr->Continuity()) << std::endl
    << "GeomAbs_C0=0, GeomAbs_G1, GeomAbs_C1, GeomAbs_G2," << std::endl
    << "GeomAbs_C2, GeomAbs_C3, GeomAbs_CN"
  );

  return ss.str();
}
} // namespace dtOO
