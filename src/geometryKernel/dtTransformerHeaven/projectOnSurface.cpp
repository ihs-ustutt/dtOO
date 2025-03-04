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

#include "projectOnSurface.h"

#include "dtTransformerFactory.h"
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomCurve_projectOnGeomSurfaceOCC.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool projectOnSurface::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(projectOnSurface, new projectOnSurface())
);

projectOnSurface::projectOnSurface() : dtTransformer() {}

projectOnSurface::~projectOnSurface() {}

dtTransformer *projectOnSurface::clone(void) const
{
  dt__throw(clone(), << "Not yet implemented.");
}

dtTransformer *projectOnSurface::create(void) const
{
  return new projectOnSurface();
}

lvH_analyticGeometry
projectOnSurface::apply(lvH_analyticGeometry const *const toTrans) const
{
  lvH_analyticGeometry retAGeo;
  for (int ii = 0; ii < toTrans->size(); ii++)
  {
    //
    // clone and cast analyticGeometry
    //
    analyticGeometry const *aGeoP = toTrans->at(ii);
    analyticCurve const *constSc;
    dt__ptrAss(constSc, analyticCurve::ConstDownCast(aGeoP));

    dt__info(
      apply(),
      << "Projecting " << dt__eval(constSc->getLabel()) << " on "
      << dt__eval(_aS->getLabel()) << "."
    );

    //
    // push translated geometry in vector
    //
    retAGeo.push_back(new analyticCurve(dt__tmpPtr(
      dtCurve,
      geomCurve_projectOnGeomSurfaceOCC(
        constSc->ptrConstDtCurve(), _aS->ptrDtSurface()
      )
        .result()
    )));
    retAGeo.back()->setLabel(constSc->getLabel());
  }
  return retAGeo;
}

bool projectOnSurface::isNecessary(void) const { return true; }

void projectOnSurface::init(
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
