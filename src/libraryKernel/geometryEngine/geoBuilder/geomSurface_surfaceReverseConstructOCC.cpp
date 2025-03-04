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

#include "geomSurface_surfaceReverseConstructOCC.h"
#include "geomSurface_baseConstructOCC.h"
#include "geometryEngine/dtOCCBSplineSurface.h"
#include "geometryEngine/dtOCCBezierSurface.h"
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>

#include <Geom_Surface.hxx>

namespace dtOO {
geomSurface_surfaceReverseConstructOCC ::geomSurface_surfaceReverseConstructOCC(
)
{
}

geomSurface_surfaceReverseConstructOCC ::geomSurface_surfaceReverseConstructOCC(
  dtSurface const *const dtS, bool const &revU, bool const &revV
)
{
  dt__ptrAss(dtOCCSurface const *const ss, dtOCCSurface::ConstDownCast(dtS));

  Geom_Surface const *sP;
  dt__mustCast(ss->OCCRef().getOCC().get(), Geom_Surface const, sP);

  Handle(Geom_Surface) aRes = Handle(Geom_Surface)::DownCast(sP->Copy());

  if (revU)
    aRes->UReverse();
  if (revV)
    aRes->VReverse();

  //
  // create new surface
  //
  dtOCCSurfaceBase base(aRes);
  _dtS.reset(geomSurface_baseConstructOCC(base).result());
}

geomSurface_surfaceReverseConstructOCC ::
  ~geomSurface_surfaceReverseConstructOCC()
{
}

dtSurface *geomSurface_surfaceReverseConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
