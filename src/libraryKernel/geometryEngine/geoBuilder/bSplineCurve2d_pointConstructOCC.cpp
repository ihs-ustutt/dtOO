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

#include "bSplineCurve2d_pointConstructOCC.h"
#include "progHelper.h"

#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCBSplineCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <logMe/logMe.h>

#include <Geom2d_BSplineCurve.hxx>
#include <Standard_Failure.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

namespace dtOO {
bSplineCurve2d_pointConstructOCC::bSplineCurve2d_pointConstructOCC(
  std::vector<dtPoint2> const &pp, dtInt const &order
)
{
  //
  // number of points
  //
  int const nP = pp.size();
  int const nOnes = nP - (order + 1);
  if (nOnes < 0)
  {
    dt__throw(
      bSplineCurve2d_pointConstructOCC(),
      << "Order too high. Please reduce order of the spline." << std::endl
      << dt__eval(order) << std::endl
      << dt__eval(nOnes)
    );
  }

  //
  // allocate arraies
  //
  TColgp_Array1OfPnt2d poles(1, nP);
  TColStd_Array1OfReal knots(1, nOnes + 2);
  TColStd_Array1OfInteger mults(1, nOnes + 2);
  mults.Init(1);

  //
  // set poles and multiplicity
  //
  dt__forAllIndex(pp, ii)
  {
    poles.SetValue(ii + 1, gp_Pnt2d(pp[ii].x(), pp[ii].y()));
  }
  for (int ii = 1; ii <= knots.Length(); ii++)
  {
    knots.SetValue(ii, static_cast<dtReal>(ii - 1));
  }
  mults.SetValue(1, order + 1);
  mults.SetValue(nOnes + 2, order + 1);

  //
  // create curve using standard constructor
  //
  Handle(Geom2d_BSplineCurve) curve =
    new Geom2d_BSplineCurve(poles, knots, mults, order);

  dtOCCCurve2dBase base;
  base.setOCC(curve);
  _dtC2d.reset(new dtOCCBSplineCurve2d(base));
}

bSplineCurve2d_pointConstructOCC::bSplineCurve2d_pointConstructOCC(
  dtPoint2 const &p0, dtPoint2 const &p1
)
{
  std::vector<dtPoint2> pp;
  pp.push_back(p0);
  pp.push_back(p1);

  _dtC2d.reset(bSplineCurve2d_pointConstructOCC(pp, 1).result());
}
bSplineCurve2d_pointConstructOCC::~bSplineCurve2d_pointConstructOCC() {}

dtCurve2d *bSplineCurve2d_pointConstructOCC::result(void)
{
  return _dtC2d->clone();
}
} // namespace dtOO
