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

#include "bSplineSurface_bSplineSurfaceSplitConstructOCC.h"

#include <logMe/logMe.h>
#include <progHelper.h>

#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>

#include <GeomConvert.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>

namespace dtOO {
bSplineSurface_bSplineSurfaceSplitConstructOCC ::
  bSplineSurface_bSplineSurfaceSplitConstructOCC(
    dtSurface const *const dtC,
    dtInt const &dir,
    dtReal const &p0,
    dtReal const &p1
  )
{
  dt__ptrAss(
    dtOCCBSplineSurface const *const bS, dtOCCBSplineSurface::ConstDownCast(dtC)
  );

  Handle(Geom_BSplineSurface) geomBSpline =
    Handle(Geom_BSplineSurface)::DownCast(bS->OCCRef().getOCC());

  dt__throwIf(
    geomBSpline.IsNull(), bSplineSurface_bSplineSurfaceSplitConstructOCC()
  );

  bool USplit = true;
  if (dir == 0)
  {
    USplit = true;
  }
  else if (dir == 1)
  {
    USplit = false;
  }
  else
    dt__throwUnexpected(bSplineSurface_bSplineSurfaceSplitConstructOCC());

  dtOCCSurfaceBase base;

  dt__tryOcc(base.setOCC(GeomConvert::SplitBSplineSurface(
    geomBSpline,
    Standard_Real(p0),
    Standard_Real(p1),
    Standard_Boolean(USplit),
    Precision::Confusion()
  ));
             , << "Split fails.");
  _dtS.reset(new dtOCCBSplineSurface(base));
}

bSplineSurface_bSplineSurfaceSplitConstructOCC ::
  ~bSplineSurface_bSplineSurfaceSplitConstructOCC()
{
}

dtSurface *bSplineSurface_bSplineSurfaceSplitConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
