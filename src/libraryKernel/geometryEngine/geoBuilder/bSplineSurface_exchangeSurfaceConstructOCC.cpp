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

#include "bSplineSurface_exchangeSurfaceConstructOCC.h"

#include "bSplineSurface_convertOCC.h"
#include "geomSurface_baseConstructOCC.h"
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <Geom_BSplineSurface.hxx>
#include <Geom_Geometry.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>

namespace dtOO {
bSplineSurface_exchangeSurfaceConstructOCC ::
  bSplineSurface_exchangeSurfaceConstructOCC(dtSurface const *const dtS)
{
  dt__pH(dtSurface) dtBS(bSplineSurface_convertOCC(dtS).result());
  //
  // cast and check surface type
  //
  Handle(Geom_BSplineSurface) cc = Handle(Geom_BSplineSurface)::DownCast(
    dtOCCBSplineSurface::ConstSecureCast(dtBS.get())->OCCRef().getOCC()->Copy()
  );

  //
  // exchange parameter space
  //
  cc->ExchangeUV();

  //
  // create new surface
  //
  dtOCCSurfaceBase base(cc);
  _dtS.reset(geomSurface_baseConstructOCC(base).result());
}

bSplineSurface_exchangeSurfaceConstructOCC ::
  ~bSplineSurface_exchangeSurfaceConstructOCC()
{
}

dtSurface *bSplineSurface_exchangeSurfaceConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
