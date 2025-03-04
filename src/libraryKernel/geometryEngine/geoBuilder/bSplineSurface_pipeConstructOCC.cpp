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

#include "bSplineSurface_pipeConstructOCC.h"
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtSurface.h>

#include "geomSurface_baseConstructOCC.h"

#include <GeomFill_Pipe.hxx>
#include <GeomLib.hxx>
#include <Geom_BoundedSurface.hxx>
#include <Geom_Geometry.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>

namespace dtOO {
bSplineSurface_pipeConstructOCC::bSplineSurface_pipeConstructOCC(
  dtCurve const *const aPath,
  dtCurve const *const aSecOne,
  dtCurve const *const aSecTwo,
  bool const &polynomial,
  dtInt const &absShape,
  dtInt const &maxDegree,
  dtInt const &nbSections
)
{
  dt__throwIf(
    !dtOCCCurve::ConstDownCast(aPath) || !dtOCCCurve::ConstDownCast(aSecOne) ||
      !dtOCCCurve::ConstDownCast(aSecTwo),
    bSplineSurface_pipeConstructOCC()
  );

  Handle(Geom_Surface) aSurface;
  dt__tryOcc(
    //
    // create pipe
    //
    GeomFill_Pipe aPipe(
      dtOCCCurve::ConstSecureCast(aPath)->OCCRef().getOCC(),
      dtOCCCurve::ConstSecureCast(aSecOne)->OCCRef().getOCC(),
      dtOCCCurve::ConstSecureCast(aSecTwo)->OCCRef().getOCC()
    );

    aPipe.Perform(
      Precision::Confusion(),
      polynomial,
      static_cast<GeomAbs_Shape>(absShape),
      maxDegree,
      nbSections
    );
    aSurface = Handle(Geom_Surface)::DownCast(aPipe.Surface()->Copy());
    , << ""
  );

  //
  // create new surface
  //
  dtOCCSurfaceBase base(aSurface);
  _dtS.reset(geomSurface_baseConstructOCC(base).result());
}

bSplineSurface_pipeConstructOCC::bSplineSurface_pipeConstructOCC(
  dtCurve const *const aPath,
  dtCurve const *const aSecOne,
  bool const &polynomial,
  dtInt const &absShape,
  dtInt const &maxDegree,
  dtInt const &nbSections

)
{
  dt__throwIf(
    !dtOCCCurve::ConstDownCast(aPath) || !dtOCCCurve::ConstDownCast(aSecOne),
    bSplineSurface_pipeConstructOCC()
  );

  Handle(Geom_Surface) aSurface;

  dt__tryOcc(
    //
    // create pipe
    //
    GeomFill_Pipe aPipe(
      dtOCCCurve::ConstSecureCast(aPath)->OCCRef().getOCC(),
      dtOCCCurve::ConstSecureCast(aSecOne)->OCCRef().getOCC()
    );

    aPipe.Perform(
      Precision::Confusion(),
      polynomial,
      static_cast<GeomAbs_Shape>(absShape),
      maxDegree,
      nbSections
    );

    aSurface = Handle(Geom_Surface)::DownCast(aPipe.Surface()->Copy());
    , << ""
  );

  //
  // create new surface
  //
  dtOCCSurfaceBase base(aSurface);
  _dtS.reset(geomSurface_baseConstructOCC(base).result());
}

bSplineSurface_pipeConstructOCC::bSplineSurface_pipeConstructOCC(
  dtCurve const *const aPath,
  dtReal const &rr,
  bool const &polynomial,
  dtInt const &absShape,
  dtInt const &maxDegree,
  dtInt const &nbSections
)
{
  dt__throwIf(
    !dtOCCCurve::ConstDownCast(aPath), bSplineSurface_pipeConstructOCC()
  );

  //
  // cast curves
  //
  //		dtOCCCurve const * const occCPath =
  //dtOCCCurve::ConstSecureCast(aPath);

  Handle(Geom_Surface) aSurface;
  dt__tryOcc(GeomFill_Pipe aPipe(
               dtOCCCurve::ConstSecureCast(aPath)->OCCRef().getOCC(),
               static_cast<Standard_Real>(rr)
  );

             aPipe.Perform(
               Precision::Confusion(),
               polynomial,
               static_cast<GeomAbs_Shape>(absShape),
               maxDegree,
               nbSections
             );

             aSurface = Handle(Geom_Surface)::DownCast(aPipe.Surface()->Copy());
             , << "");

  //
  // create new surface
  //
  dtOCCSurfaceBase base(aSurface);
  _dtS.reset(geomSurface_baseConstructOCC(base).result());
}

bSplineSurface_pipeConstructOCC::~bSplineSurface_pipeConstructOCC() {}

dtSurface *bSplineSurface_pipeConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
