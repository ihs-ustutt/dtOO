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

#include "scale.h"

#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include "dtTransformerFactory.h"
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map1dTo3dTransformed.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3dTransformed.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3dTransformed.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>

namespace dtOO {
bool scale::_registrated =
  dtTransformerFactory::registrate(dt__tmpPtr(scale, new scale()));

scale::scale() : dtTransformerInvThreeD() {}

scale::~scale() {}

scale::scale(const scale &orig) : dtTransformerInvThreeD(orig)
{
  _scale = orig._scale;
}

dtTransformerInvThreeD *scale::clone(void) const { return new scale(*this); }

dtTransformerInvThreeD *scale::create(void) const { return new scale(); }

std::vector<dtPoint3> scale::apply(std::vector<dtPoint3> const *const toTrans
) const
{
  std::vector<dtPoint3> ret(toTrans->size());
  dt__forAllIndex(*toTrans, ii)
  {
    ret[ii] = dtPoint3(
      _scale.x() * toTrans->at(ii).x(),
      _scale.y() * toTrans->at(ii).y(),
      _scale.z() * toTrans->at(ii).z()
    );
  }

  return ret;
}

std::vector<dtPoint3> scale::retract(std::vector<dtPoint3> const *const toTrans
) const
{
  std::vector<dtPoint3> ret(toTrans->size());
  dt__forAllIndex(*toTrans, ii)
  {
    ret[ii] = dtPoint3(
      toTrans->at(ii).x() / _scale.x(),
      toTrans->at(ii).y() / _scale.y(),
      toTrans->at(ii).z() / _scale.z()
    );
  }

  return ret;
}

std::vector<dtVector3> scale::apply(std::vector<dtVector3> const *const toTrans
) const
{
  std::vector<dtVector3> ret(toTrans->size());
  dt__forAllIndex(*toTrans, ii)
  {
    ret[ii] = dtVector3(
      _scale.x() * toTrans->at(ii).x(),
      _scale.y() * toTrans->at(ii).y(),
      _scale.z() * toTrans->at(ii).z()
    );
  }

  return ret;
}

std::vector<dtVector3>
scale::retract(std::vector<dtVector3> const *const toTrans) const
{
  std::vector<dtVector3> ret(toTrans->size());
  dt__forAllIndex(*toTrans, ii)
  {
    ret[ii] = dtVector3(
      toTrans->at(ii).x() / _scale.x(),
      toTrans->at(ii).y() / _scale.y(),
      toTrans->at(ii).z() / _scale.z()
    );
  }

  return ret;
}

bool scale::isNecessary(void) const { return true; }

void scale::init(
  ::QDomElement const *tE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG
)
{
  dtTransformerInvThreeD::init(tE, bC, cV, aF, aG);

  dt__throwIf(!dtXmlParserBase::hasAttribute("scale_vector", *tE), init());

  _scale = dtXmlParserBase::getDtVector3(
    dtXmlParserBase::getAttributeStr("scale_vector", *tE), bC
  );
  dt__info(init(), << "Init transfomer with " << dt__eval(_scale) << ".");
}
} // namespace dtOO
