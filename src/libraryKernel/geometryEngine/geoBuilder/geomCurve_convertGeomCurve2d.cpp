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

#include "geomCurve_convertGeomCurve2d.h"
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include "geomCurve_baseConstructOCC.h"

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/dtOCCCurve2dBase.h>
#include <geometryEngine/dtOCCCurveBase.h>

#include <GeomLib.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>

namespace dtOO {
geomCurve_convertGeomCurve2d::geomCurve_convertGeomCurve2d(
  dtCurve2d const *const dtC2d, dtPoint3 const &pp, dtVector3 const &vv
)
{
  dt__ptrAss(
    dtOCCCurve2d const *const occC2d, dtOCCCurve2d::ConstDownCast(dtC2d)
  );

  gp_Ax2 ax2(gp_Pnt(pp.x(), pp.y(), pp.z()), gp_Dir(vv.x(), vv.y(), vv.z()));
  Handle(Geom_Curve) occC = GeomLib::To3d(ax2, occC2d->OCCRef().getOCC());

  dtOCCCurveBase base;
  base.setOCC(occC);
  _dtC.reset(geomCurve_baseConstructOCC(base).result());
  //		if ( !(Handle(Geom_BSplineCurve)::DownCast(occC).IsNull()) ) {
  //		  _dtC.reset( new dtOCCBSplineCurve(base) );
  //		}
  //		else if ( !(Handle(Geom_BezierCurve)::DownCast(occC).IsNull()) )
  //{ 		  _dtC.reset( new dtOCCBezierCurve(base) );
  //		}
  //		else if ( !(Handle(Geom_TrimmedCurve)::DownCast(occC).IsNull())
  //) { 			dt__ptrAss(
  // dtOCCTrimmedCurve2d const * const tC2d,
  //				dtOCCTrimmedCurve2d::ConstDownCast(dtC2d)
  //			);
  //		  _dtC.reset( new dtOCCTrimmedCurve(base, tC2d->getU0(),
  // tC2d->getU1() ) );
  //		}
  //		else if ( !(Handle(Geom_Line)::DownCast(occC).IsNull()) ) {
  //		  _dtC.reset( new dtOCCLine(base) );
  //		}
  //		else {
  //			dt__throw(
  //			  geomCurve_convertGeomCurve2d(),
  //			  << "Unknown OCC type."
  //			);
  //		}
}

geomCurve_convertGeomCurve2d::~geomCurve_convertGeomCurve2d() {}

dtCurve *geomCurve_convertGeomCurve2d::result(void) { return _dtC->clone(); }
} // namespace dtOO
