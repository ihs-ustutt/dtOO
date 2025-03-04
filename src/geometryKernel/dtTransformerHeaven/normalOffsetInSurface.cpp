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

#include "normalOffsetInSurface.h"
#include "dtTransformerFactory.h"
#include <analyticGeometryHeaven/aGBuilder/map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool normalOffsetInSurface::_registrated = dtTransformerFactory::registrate(
  dt__tmpPtr(normalOffsetInSurface, new normalOffsetInSurface())
);

normalOffsetInSurface::normalOffsetInSurface() : dtTransformer() {}

normalOffsetInSurface::normalOffsetInSurface(normalOffsetInSurface const &orig)
{
  _m2d.reset(orig._m2d->clone());
  _thickness = orig._thickness;
  _nPoints = orig._nPoints;
  _nIntPoints = orig._nIntPoints;
}

normalOffsetInSurface::~normalOffsetInSurface() {}

dtTransformer *normalOffsetInSurface::clone(void) const
{
  return new normalOffsetInSurface(*this);
}

dtTransformer *normalOffsetInSurface::create(void) const
{
  return new normalOffsetInSurface();
}

lvH_analyticGeometry
normalOffsetInSurface::apply(lvH_analyticGeometry const *const aGeoVecP) const
{
  lvH_analyticGeometry retAGeo;
  for (int ii = 0; ii < aGeoVecP->size(); ii++)
  {
    //
    // cast analyticGeometry
    //
    dt__ptrAss(
      map1dTo3d const *const m1d, map1dTo3d::ConstDownCast(aGeoVecP->at(ii))
    );

    retAGeo.push_back(
      map1dTo3d_normalOffsetMap1dTo3dInMap2dTo3d(
        m1d, _m2d.get(), _thickness, _nPoints, _nIntPoints, _order
      )
        .result()
    );

    retAGeo.back()->setLabel(m1d->getLabel());
  }
  return retAGeo;
}

bool normalOffsetInSurface::isNecessary(void) const { return true; }

void normalOffsetInSurface::init(
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
    _m2d.reset(map2dTo3d::ConstDownCast(
      aG->get(dtXmlParserBase::getAttributeStr("part_label", *tE))->clone()
    ));
  }
  else
  {
    dt__throw(
      init(), << dt__eval(dtXmlParserBase::hasAttribute("part_label", *tE))
    );
  }

  if (dtXmlParserBase::hasAttribute("thickness", *tE))
  {
    _thickness =
      dtXmlParserBase::getAttributeFloatMuParse("thickness", *tE, cV, aF);
  }
  else
  {
    dt__throw(
      init(), << dt__eval(dtXmlParserBase::hasAttribute("thickness", *tE))
    );
  }

  if (dtXmlParserBase::hasAttribute("number_points", *tE))
  {
    _nPoints =
      dtXmlParserBase::getAttributeIntMuParse("number_points", *tE, cV, aF);
  }
  else
  {
    dt__throw(
      init(), << dt__eval(dtXmlParserBase::hasAttribute("number_points", *tE))
    );
  }

  if (dtXmlParserBase::hasAttribute("number_integration_points", *tE))
  {
    _nIntPoints = dtXmlParserBase::getAttributeIntMuParse(
      "number_integration_points", *tE, cV, aF
    );
  }
  else
  {
    dt__throw(
      init(),
      << dt__eval(
           dtXmlParserBase::hasAttribute("number_integration_points", *tE)
         )
    );
  }
  if (dtXmlParserBase::hasAttribute("order", *tE))
  {
    _order = dtXmlParserBase::getAttributeIntMuParse("order", *tE, cV, aF);
  }
  else
  {
    dt__throw(init(), << dt__eval(dtXmlParserBase::hasAttribute("order", *tE)));
  }
}
} // namespace dtOO
