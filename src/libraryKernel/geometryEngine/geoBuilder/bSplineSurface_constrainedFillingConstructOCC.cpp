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

#include "bSplineSurface_constrainedFillingConstructOCC.h"

#include <GeomFill_Boundary.hxx>
#include <GeomFill_ConstrainedFilling.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCGeomFillBoundary.h>
#include <geometryEngine/dtOCCGeomFillBoundaryBase.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomFillSimpleBound_curveConstructOCC.h>
#include <logMe/logMe.h>

namespace dtOO {
bSplineSurface_constrainedFillingConstructOCC ::
  bSplineSurface_constrainedFillingConstructOCC(
    dtCurve const *c0,
    dtCurve const *c1,
    dtCurve const *c2,
    dtCurve const *c3,
    dtInt const &maxDeg,
    dtInt const &maxSeg
  )
{
  dt__vH(dtCurve const *) cV;
  cV.push_back(c0);
  cV.push_back(c1);
  cV.push_back(c2);
  cV.push_back(c3);
  dt__pVH(dtCurve) cCV;
  dt__forAllIndex(cV, ii)
  {
    if (!dtOCCGeomFillBoundary::Is(cV[ii]))
    {
      cCV.push_back(geomFillSimpleBound_curveConstructOCC(cV[ii]).result());
    }
    else
      cCV.push_back(cV[ii]->clone());
  }

  GeomFill_ConstrainedFilling filler(
    static_cast<Standard_Integer>(maxDeg), static_cast<Standard_Integer>(maxSeg)
  );

  dt__tryOcc(filler.Init(
    dtOCCGeomFillBoundary::ConstDownCast(&(cCV[0]))->OCCBoundaryRef().getOCC(),
    dtOCCGeomFillBoundary::ConstDownCast(&(cCV[1]))->OCCBoundaryRef().getOCC(),
    dtOCCGeomFillBoundary::ConstDownCast(&(cCV[2]))->OCCBoundaryRef().getOCC(),
    dtOCCGeomFillBoundary::ConstDownCast(&(cCV[3]))->OCCBoundaryRef().getOCC()
  );
             , << "");

  _dtS.reset(new dtOCCBSplineSurface(dtOCCSurfaceBase(Handle(Geom_Surface
  )::DownCast(filler.Surface()->Copy()))));
}

bSplineSurface_constrainedFillingConstructOCC ::
  ~bSplineSurface_constrainedFillingConstructOCC()
{
}

dtSurface *bSplineSurface_constrainedFillingConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
