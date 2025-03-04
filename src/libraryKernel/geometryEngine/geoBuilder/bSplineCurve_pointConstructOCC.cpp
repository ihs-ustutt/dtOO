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

#include "bSplineCurve_pointConstructOCC.h"
#include "progHelper.h"

#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <logMe/logMe.h>

#include <Geom_BSplineCurve.hxx>
#include <Standard_Failure.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>

namespace dtOO {
bSplineCurve_pointConstructOCC::bSplineCurve_pointConstructOCC(
  std::vector<dtPoint3> const &pp, dtInt const &order
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
      bSplineCurve_pointConstructOCC(),
      << "Order too high. Please reduce order of the spline." << std::endl
      << dt__eval(order) << std::endl
      << dt__eval(nOnes)
    );
  }

  //
  // allocate arraies
  //
  TColgp_Array1OfPnt poles(1, nP);
  TColStd_Array1OfReal knots(1, nOnes + 2);
  TColStd_Array1OfInteger mults(1, nOnes + 2);
  mults.Init(1);

  //
  // set poles and multiplicity
  //
  dt__forAllIndex(pp, ii)
  {
    poles.SetValue(ii + 1, gp_Pnt(pp[ii].x(), pp[ii].y(), pp[ii].z()));
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
  Handle(Geom_BSplineCurve) curve =
    new Geom_BSplineCurve(poles, knots, mults, order);

  dtOCCCurveBase base;
  base.setOCC(curve);
  _dtC.reset(new dtOCCBSplineCurve(base));

  //		std::vector< dtReal > ul;
  //		dtReal length = _dtC->l_uPercent(1.);
  //		ul.push_back(0.);
  //		ul.push_back( _dtC->l_uPercent(0.)/length );
  //		ul.push_back(.25);
  //		ul.push_back( _dtC->l_uPercent(.25)/length );
  //		ul.push_back(.5);
  //		ul.push_back( _dtC->l_uPercent(.5)/length );
  //		ul.push_back(.75);
  //		ul.push_back( _dtC->l_uPercent(.75)/length );
  //		ul.push_back(1.00);
  //		ul.push_back( _dtC->l_uPercent(1.00)/length );

  //		std::vector< std::string > header;
  //		header.push_back("u");
  //		header.push_back("l(u)/l_ges");
  //		dt__debug(
  //			bSplineCurve_pointConstructOCC(),
  //			<< floatVecToTable(header, ul)
  //		);
}

bSplineCurve_pointConstructOCC::bSplineCurve_pointConstructOCC(
  dtPoint3 const &p0, dtPoint3 const &p1
)
{
  std::vector<dtPoint3> pp(2);
  pp[0] = p0;
  pp[1] = p1;

  _dtC.reset(bSplineCurve_pointConstructOCC(pp, 1).result());
}

bSplineCurve_pointConstructOCC::~bSplineCurve_pointConstructOCC() {}

dtCurve *bSplineCurve_pointConstructOCC::result(void) { return _dtC->clone(); }
} // namespace dtOO
