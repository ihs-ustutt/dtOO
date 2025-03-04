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

#include "bSplineSurface_geomCurveFillConstructOCC.h"

#include "bSplineCurve_convertOCC.h"
#include "bSplineCurve_pointConstructOCC.h"
#include "bSplineSurface_bSplineCurveFillConstructOCC.h"
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Precision.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Standard_Failure.hxx>
#include <Standard_TypeDef.hxx>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtOCCBSplineCurve.h>
#include <geometryEngine/dtOCCCurve.h>
#include <geometryEngine/dtOCCCurveBase.h>
#include <geometryEngine/dtOCCSurfaceBase.h>
#include <geometryEngine/dtOCCTrimmedCurve.h>
#include <geometryEngine/dtSurface.h>
#include <logMe/logMe.h>

namespace dtOO {
bSplineSurface_geomCurveFillConstructOCC::
  bSplineSurface_geomCurveFillConstructOCC(
    vectorHandling<dtCurve const *> const &cc
  )
{
  init(cc);
}

bSplineSurface_geomCurveFillConstructOCC::
  bSplineSurface_geomCurveFillConstructOCC(dt__pVH(dtCurve) const &cc)
{
  vectorHandling<dtCurve const *> ccV;
  for (dt__pVH(dtCurve)::const_iterator it = cc.begin(); it != cc.end(); ++it)
  {
    ccV.push_back(&(*it));
  }

  init(ccV);
}

bSplineSurface_geomCurveFillConstructOCC::
  bSplineSurface_geomCurveFillConstructOCC(
    dtCurve const *c0, dtCurve const *c1, dtCurve const *c2, dtCurve const *c3
  )
{
  vectorHandling<dtCurve const *> cc(4);
  cc[0] = c0;
  cc[1] = c1;
  cc[2] = c2;
  cc[3] = c3;

  init(cc);
}

void bSplineSurface_geomCurveFillConstructOCC::init(
  vectorHandling<dtCurve const *> const &cc
)
{
  dt__throwIfWithMessage(
    cc.size() != 4,
    bSplineSurface_geomCurveFillConstructOCC(),
    << "Only supported with 4 curves." << std::endl
    << dt__eval(cc.size())
  );
  vectorHandling<dtCurve const *> C(4);
  dt__forFromToIndex(0, 4, ii)
  {
    C[ii] = bSplineCurve_convertOCC(*(cc[ii])).result();
    //			dtOCCBSplineCurve const * occBsc =
    //dtOCCBSplineCurve::ConstDownCast(cc[ii]); 			dtOCCTrimmedCurve const * occTc
    //= dtOCCTrimmedCurve::ConstDownCast(cc[ii]); 			if (occBsc) { 				C[ii] =
    //occBsc->clone();
    //			}
    //			else if (occTc) {
    //  			C[ii] = occTc->bSplineCurve();
    //			}
    //      else {
    //				dt__throw(
    //				  bSplineSurface_geomCurveFillConstructOCC(),
    //					<< dt__eval(occBsc) << std::endl
    //					<< dt__eval(occTc)
    //				);
    //			}
  }

  _dtS.reset(bSplineSurface_bSplineCurveFillConstructOCC(C).result());

  C.destroy();
}

bSplineSurface_geomCurveFillConstructOCC::
  ~bSplineSurface_geomCurveFillConstructOCC()
{
}

dtSurface *bSplineSurface_geomCurveFillConstructOCC::result(void)
{
  return _dtS->clone();
}
} // namespace dtOO
