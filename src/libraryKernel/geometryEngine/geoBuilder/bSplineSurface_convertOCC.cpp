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

#include "bSplineSurface_convertOCC.h"

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include "geomSurface_baseConstructOCC.h"
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>

#include <GeomConvert.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_Surface.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>

namespace dtOO {
bSplineSurface_convertOCC::bSplineSurface_convertOCC(dtSurface const *const dtS)
{
  //
  // already a BSplineSurface
  //
  if (dtOCCBSplineSurface::Is(dtS))
  {
    _dtS.reset(dtS->clone());
    return;
  }

  //
  // cast and check surface type
  //
  Handle(Geom_Surface) ss = Handle(Geom_Surface
  )::DownCast(dtOCCSurface::MustConstDownCast(dtS)->OCCRef().getOCC()->Copy());
  Handle(Geom_BSplineSurface) bS;
  dt__tryOcc(bS = GeomConvert::SurfaceToBSplineSurface(ss);, << "");

  //
  // create new surface
  //
  dtOCCSurfaceBase base(bS);
  _dtS.reset(geomSurface_baseConstructOCC(base).result());
}

bSplineSurface_convertOCC::~bSplineSurface_convertOCC() {}

dtSurface *bSplineSurface_convertOCC::result(void) { return _dtS->clone(); }
} // namespace dtOO
