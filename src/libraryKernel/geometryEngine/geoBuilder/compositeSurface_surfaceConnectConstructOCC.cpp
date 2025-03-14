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

#include "compositeSurface_surfaceConnectConstructOCC.h"

#include <geometryEngine/dtOCCCompositeSurface.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/twoDArrayHandling.h>
#include <logMe/logMe.h>

#include <Precision.hxx>
#include <ShapeExtend_CompositeSurface.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <TColGeom_HArray2OfSurface.hxx>

namespace dtOO {
compositeSurface_surfaceConnectConstructOCC ::
  compositeSurface_surfaceConnectConstructOCC(
    twoDArrayHandling<dtSurface const *> const &ss
  )
{
  Handle(TColGeom_HArray2OfSurface) grid =
    new TColGeom_HArray2OfSurface(1, ss.size(0), 1, ss.size(1));

  dt__forAllIndex(ss, ii)
  {
    dt__forAllIndex(ss[ii], jj)
    {
      dtOCCSurface const *const occS = dtOCCSurface::ConstDownCast(ss[ii][jj]);
      dt__throwIf(!occS, compositeSurface_surfaceConnectConstructOCC());
      dt__tryOcc(grid->SetValue(ii + 1, jj + 1, occS->OCCRef().getOCC());
                 , << "Cannot set grid(" << ii + 1 << ", " << jj + 1 << ").");
    }
  }
  Handle(ShapeExtend_CompositeSurface) compSurf;
  dt__tryOcc(compSurf = new ShapeExtend_CompositeSurface(grid);
             , << "Cannot create ShapeExtend_CompositeSurface");

  dtOCCSurfaceBase baseS(compSurf);
  _dtS.reset(new dtOCCCompositeSurface(baseS));
}

compositeSurface_surfaceConnectConstructOCC ::
  ~compositeSurface_surfaceConnectConstructOCC()
{
}

dtSurface *compositeSurface_surfaceConnectConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
