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

#include "bSplineSurface_skinConstructOCC.h"

#include <geometryEngine/dtOCCBSplineSurface.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

#include <BSplCLib.hxx>
#include <GeomFill_AppSurf.hxx>
#include <GeomFill_Line.hxx>
#include <GeomFill_SectionGenerator.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>

namespace dtOO {
bSplineSurface_skinConstructOCC::bSplineSurface_skinConstructOCC() {}

bSplineSurface_skinConstructOCC::bSplineSurface_skinConstructOCC(
  dtCurve const *const c0, dtCurve const *const c1
)
{
  vectorHandling<dtCurve const *> cL;
  cL.push_back(c0);
  cL.push_back(c1);
  _dtS.reset(bSplineSurface_skinConstructOCC(cL).result());
}

bSplineSurface_skinConstructOCC::bSplineSurface_skinConstructOCC(
  vectorHandling<dtCurve const *> const &cL
)
{
  _dtS.reset(
    bSplineSurface_skinConstructOCC(cL, 1, BSplCLib::MaxDegree(), 0).result()
  );
}

bSplineSurface_skinConstructOCC::bSplineSurface_skinConstructOCC(
  vectorHandling<dtCurve const *> const &cL,
  dtInt const &minDeg,
  dtInt const &maxDeg,
  dtInt const &nIter
)
{
  GeomFill_SectionGenerator aSecGen;
  dt__forAllIndex(cL, ii)
  {
    dt__ptrAss(dtOCCCurve * bC, dtOCCCurve::DownCast(cL[ii]->clone()));
    dt__tryOcc(aSecGen.AddCurve(bC->OCCRef().getOCC());, << "");
    delete bC;
  }
  dt__tryOcc(aSecGen.Perform(Precision::Parametric(Precision::Confusion()));
             , << "");

  Handle(GeomFill_Line) aLine = new GeomFill_Line(cL.size());
  const Standard_Integer aMinDeg = minDeg;
  const Standard_Integer aMaxDeg = maxDeg;
  const Standard_Integer aNbIt = nIter;
  Standard_Real aTol3d = Precision::Confusion();
  Standard_Real aTol2d = Precision::Parametric(aTol3d);

  GeomFill_AppSurf anAlgo;
  dt__tryOcc(anAlgo.Init(aMinDeg, aMaxDeg, aTol3d, aTol2d, aNbIt);, << "");
  dt__tryOcc(anAlgo.Perform(aLine, aSecGen);, << "");

  Handle(Geom_Surface) aRes;
  dt__tryOcc(aRes = new Geom_BSplineSurface(
               anAlgo.SurfPoles(),
               anAlgo.SurfWeights(),
               anAlgo.SurfUKnots(),
               anAlgo.SurfVKnots(),
               anAlgo.SurfUMults(),
               anAlgo.SurfVMults(),
               anAlgo.UDegree(),
               anAlgo.VDegree()
             );
             , << "");

  dtOCCSurfaceBase base;
  base.setOCC(aRes);

  _dtS.reset(new dtOCCBSplineSurface(base));
}

bSplineSurface_skinConstructOCC::~bSplineSurface_skinConstructOCC() {}

dtSurface *bSplineSurface_skinConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
