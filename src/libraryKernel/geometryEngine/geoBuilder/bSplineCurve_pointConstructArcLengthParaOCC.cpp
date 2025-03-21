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

#include "bSplineCurve_pointConstructArcLengthParaOCC.h"

#include <Approx_CurvilinearParameter.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <logMe/logMe.h>

namespace dtOO {
bSplineCurve_pointConstructArcLengthParaOCC ::
  bSplineCurve_pointConstructArcLengthParaOCC(
    dtCurve const *dtC,
    dtReal const tol,
    int const maxOrder,
    dtInt const segments
  )
{
  const Handle(Geom_BSplineCurve) bsc = Handle(Geom_BSplineCurve
  )::DownCast(dtOCCCurve::ConstDownCast(dtC)->OCCRef().getOCC());

  Handle(GeomAdaptor_Curve) gac;
  dt__tryOcc(gac = new GeomAdaptor_Curve(bsc);, << "");
  //		gahc.Load( bsc );

  // Standard_Real llR = static_cast<Standard_Real>(length);
  Handle(Geom_BSplineCurve) curve;

  dt__tryOcc(Approx_CurvilinearParameter acp(
               gac,
               static_cast<Standard_Real>(tol),
               GeomAbs_Shape::GeomAbs_C0,
               static_cast<Standard_Integer>(maxOrder),
               static_cast<Standard_Integer>(segments)
  );
             curve = acp.Curve3d();
             , << "");

  dtOCCCurveBase base;
  base.setOCC(curve);
  _dtC.reset(new dtOCCBSplineCurve(base));

  std::vector<dtReal> ul;
  dtReal length = _dtC->l_uPercent(1.);
  ul.push_back(0.);
  ul.push_back(_dtC->l_uPercent(0.) / length);
  ul.push_back(.25);
  ul.push_back(_dtC->l_uPercent(.25) / length);
  ul.push_back(.5);
  ul.push_back(_dtC->l_uPercent(.5) / length);
  ul.push_back(.75);
  ul.push_back(_dtC->l_uPercent(.75) / length);
  ul.push_back(1.00);
  ul.push_back(_dtC->l_uPercent(1.00) / length);

  std::vector<std::string> header;
  header.push_back("u");
  header.push_back("l(u)/l_ges");
  dt__debug(
    bSplineCurve_pointConstructArcLengthParaOCC(),
    << logMe::vecToTable(header, ul)
  );
}

bSplineCurve_pointConstructArcLengthParaOCC ::
  ~bSplineCurve_pointConstructArcLengthParaOCC()
{
}

dtCurve *bSplineCurve_pointConstructArcLengthParaOCC::result(void)
{
  return _dtC->clone();
}
} // namespace dtOO
