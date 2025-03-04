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

#include "dtOCCCurve.h"

#include "dtOCCCurveBase.h"
#include <interfaceHeaven/calculationTypeHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <Extrema_ExtPC.hxx>
#include <Extrema_POnCurv.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GeomConvert_CompCurveToBSplineCurve.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_Curve.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

namespace dtOO {
dtOCCCurve::dtOCCCurve()
{
  _curve.reset(new dtOCCCurveBase());
  _ptr = NULL;
}

dtOCCCurve::dtOCCCurve(dtOCCCurveBase const &orig)
{
  _curve.reset(new dtOCCCurveBase());
  _curve->setOCC(Handle(Geom_Curve)::DownCast(orig.getOCC()->Copy()));
  dt__mustCast(OCCRef().getOCC().get(), Geom_Curve const, _ptr);
}

dtOCCCurve::~dtOCCCurve() {}

dtReal dtOCCCurve::minPara(dtInt const &dir) const
{
  dt__throwIf(dir != 0, minPara());
  Standard_Real U1;
  Standard_Real U2;
  dt__tryOcc(U1 = _ptr->FirstParameter(); U2 = _ptr->LastParameter();
             return floatHandling::nextIfSmaller(U1);
             ,
             << dt__eval(U1) << std::endl
             << dt__eval(U2));
}

dtReal dtOCCCurve::maxPara(dtInt const &dir) const
{
  dt__throwIf(dir != 0, maxPara());
  Standard_Real U1;
  Standard_Real U2;
  dt__tryOcc(U1 = _ptr->FirstParameter(); U2 = _ptr->LastParameter();
             return floatHandling::prevIfBigger(U2);
             ,
             << dt__eval(U1) << std::endl
             << dt__eval(U2));
}

bool dtOCCCurve::closed(void) const
{
  return static_cast<bool>(_ptr->IsClosed());
}

dtPoint3 dtOCCCurve::point(dtReal const uu) const
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

dtVector3 dtOCCCurve::firstDer(dtReal const uu) const
{
  Standard_Real uR = static_cast<Standard_Real>(uu);
  gp_Pnt pp;
  gp_Vec vv;

  dt__tryOcc(_ptr->D1(uR, pp, vv);, << dt__eval(uu));

  return dtVector3(
    static_cast<dtReal>(vv.Coord(1)),
    static_cast<dtReal>(vv.Coord(2)),
    static_cast<dtReal>(vv.Coord(3))
  );
}

/**
 * @todo Make tolerance adjustable or better automatically adjustable.
 */
dtReal dtOCCCurve::l_u(dtReal const uu) const
{
  GeomAdaptor_Curve gac;
  gac.Load(_curve->getOCC());

  Standard_Real uuR = static_cast<Standard_Real>(uu);
  Standard_Real ll;
  dt__tryOcc(ll = GCPnts_AbscissaPoint::Length(
               gac, _ptr->FirstParameter(), uuR, Precision::Confusion()
             );
             , << dt__eval(uu));

  return static_cast<dtReal>(ll);
}

/**
 * @todo Make tolerance adjustable or better automatically adjustable.
 */
dtReal dtOCCCurve::u_l(dtReal const length) const
{
  GeomAdaptor_Curve gac;
  gac.Load(_curve->getOCC());

  Standard_Real llR = static_cast<Standard_Real>(length);
  Standard_Real uu;
  Standard_Real uI = getUMin() + .5 * (getUMax() - getUMin());
  dt__tryOcc(GCPnts_AbscissaPoint ap(
               gac, llR, _ptr->FirstParameter(), uI, Precision::Confusion()
  );
             uu = ap.Parameter();
             ,
             << dt__eval(length) << std::endl
             << dt__eval(uu));

  return static_cast<dtReal>(uu);
}

dtReal dtOCCCurve::reparam(dtPoint3 const point) const
{
  gp_Pnt pp(
    static_cast<Standard_Real>(point.x()),
    static_cast<Standard_Real>(point.y()),
    static_cast<Standard_Real>(point.z())
  );
  GeomAdaptor_Curve gac;
  gac.Load(_curve->getOCC());
  Extrema_ExtPC ext(pp, gac);

  if (ext.NbExt() < 1)
  {
    dt__warning(
      reparam(),
      << "Reparameterization fails!" << std::endl
      << "ext.NbExt() = " << ext.NbExt() << std::endl
      << "Point = ( " << point << " )" << std::endl
      << "( " << this->pointPercent(0.0) << " )---( " << this->pointPercent(1.0)
      << " )" << std::endl
    );

    return dtCurve::reparam(point);
  }

  Extrema_POnCurv epp = ext.Point(1);
  return static_cast<dtReal>(epp.Parameter());
}

dtInt dtOCCCurve::continuity(void) const
{
  dtInt ret = -1;
  dt__tryOcc(ret = static_cast<dtInt>(_ptr->Continuity());, << dt__eval(ret));
  return ret;
}

std::string dtOCCCurve::dumpToString(void) const
{
  std::stringstream ss;

  ss << dt__dumpToString(<< dt__eval(_curve->dumpToString()));

  return ss.str();
}

void dtOCCCurve::revert(void)
{
  Handle(Geom_Curve) rev = _ptr->Reversed();
  _curve->setOCC(rev);
  dt__mustCast(OCCRef().getOCC().get(), Geom_Curve const, _ptr);
}

void dtOCCCurve::translate(dtVector3 const &tt)
{
  gp_Vec vv(
    static_cast<Standard_Real>(tt.x()),
    static_cast<Standard_Real>(tt.y()),
    static_cast<Standard_Real>(tt.z())
  );
  OCCRef().getOCC()->Translate(vv);
}

dtOCCCurveBase const &dtOCCCurve::OCCRef(void) const { return *(_curve.get()); }

dtOCCCurveBase &dtOCCCurve::OCCRef(void) { return *(_curve.get()); }
dt__C_addCloneForpVH(dtOCCCurve);
} // namespace dtOO
