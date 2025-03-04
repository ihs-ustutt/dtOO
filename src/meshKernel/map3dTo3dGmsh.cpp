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

#include "map3dTo3dGmsh.h"

#include "boundedVolumeFactory.h"
#include "logMe/dtMacros.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <baseContainerHeaven/baseContainer.h>
#include <constValueHeaven/constValue.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>

namespace dtOO {
bool map3dTo3dGmsh::_registrated = boundedVolumeFactory::registrate(
  dt__tmpPtr(map3dTo3dGmsh, new map3dTo3dGmsh())
);

map3dTo3dGmsh::map3dTo3dGmsh() : gmshBoundedVolume() {}

map3dTo3dGmsh::~map3dTo3dGmsh() {}

void map3dTo3dGmsh::jInit(
  jsonPrimitive const &jE,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV
)
{
  //
  // init gmshBoundedVolume
  //
  gmshBoundedVolume::jInit(jE, bC, cV, aF, aG, bV);

  //
  // set current model
  //
  ::GModel::setCurrent(_gm);

  //
  // region
  //
  dt__forAllRefAuto(jE.lookupVecClone<analyticGeometry>("", aG), anAG)
  {
    dtInt vId;
    _gm->addIfRegionToGmshModel(map3dTo3d::ConstDownCast(&anAG), &vId);
  }
}

void map3dTo3dGmsh::init(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV
)
{
  gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);

  std::vector<jsonPrimitive> aGs;
  dt__forAllRefAuto(
    qtXmlPrimitive::getChildVector("analyticGeometry", element), theXml
  )
  {
    aGs.push_back(jsonPrimitive().append<std::string>(
      "label", qtXmlPrimitive::getAttributeStr("label", theXml)
    ));
  }

  map3dTo3dGmsh::jInit(
    jsonPrimitive().append<std::vector<jsonPrimitive>>("analyticGeometry", aGs),
    bC,
    cV,
    aF,
    aG,
    bV
  );
}
} // namespace dtOO
