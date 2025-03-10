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

#include "reparamInSurface.h"
#include "analyticGeometryHeaven/vec2dOneDInMap2dTo3d.h"
#include "dtTransformerFactory.h"
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomCurve2d_reparamInGeomSurfaceOCC.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool reparamInSurface::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(reparamInSurface, new reparamInSurface())
);

reparamInSurface::reparamInSurface() : dtTransformer() {}

reparamInSurface::~reparamInSurface() {}

dtTransformer *reparamInSurface::clone(void) const
{
  dt__throw(clone(), << "Not yet implemented.");
}

dtTransformer *reparamInSurface::create(void) const
{
  return new reparamInSurface();
}

lvH_analyticGeometry
reparamInSurface::apply(lvH_analyticGeometry const *const aGeoVecP) const
{
  lvH_analyticGeometry retAGeo;
  for (int ii = 0; ii < aGeoVecP->size(); ii++)
  {
    //
    // clone and cast analyticGeometry
    //
    analyticGeometry const *aGeoP = aGeoVecP->at(ii);
    analyticCurve const *constSc;
    dt__ptrAss(constSc, analyticCurve::ConstDownCast(aGeoP));

    dt__info(
      apply(),
      << "Reparam " << dt__eval(constSc->getLabel()) << " on "
      << dt__eval(_aS->getLabel()) << "."
    );

    //
    // push translated geometry in vector
    //
    dt__pH(dtCurve2d) dtC2d(geomCurve2d_reparamInGeomSurfaceOCC(
                              constSc->ptrConstDtCurve(), _aS->ptrDtSurface()
    )
                              .result());
    dt__pH(vec2dCurve2dOneD) v2d1d(new vec2dCurve2dOneD(dtC2d.get()));
    retAGeo.push_back(new vec2dOneDInMap2dTo3d(v2d1d.get(), _aS.get()));

    retAGeo.back()->setLabel(constSc->getLabel());
  }
  return retAGeo;
}

bool reparamInSurface::isNecessary(void) const { return true; }

void reparamInSurface::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformer::init(tE, bC, cV, aF, aG);

  if (dtXmlParserBase::hasAttribute("part_label", *tE))
  {
    _aS.reset(analyticSurface::ConstDownCast(
      aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))->clone()
    ));
  }
  else
  {
    dt__throw(
      init(), << dt__eval(dtXmlParserBase::hasAttribute("part_label", *tE))
    );
  }
}
} // namespace dtOO
