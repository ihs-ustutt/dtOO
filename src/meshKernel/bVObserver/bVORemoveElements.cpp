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

#include "bVORemoveElements.h"

#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>

#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshModel.h>
#include <vector>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVORemoveElements::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVORemoveElements, new bVORemoveElements())
);

bVORemoveElements::bVORemoveElements() {}

bVORemoveElements::~bVORemoveElements() {}

void bVORemoveElements::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  boundedVolume *attachTo
)
{
  //
  // init bVOInterface
  //
  bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);

  // <bVObserver
  //   name="bVORemoveElements"
  //   label="{name0}"
  // />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::vector<std::string>>(
    "_label", dtXmlParserBase::getAttributeStrVector("label", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVORemoveElements::postUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  logContainer<bVORemoveElements> logC(logINFO, "postUpdate()");
  dt__forAllRefAuto(config().lookup<std::vector<std::string>>("_label"), aLabel)
  {
    dt__forAllRefAuto(gm->dtEdges(), aEdge)
    {
      if (gm->matchWildCardPhysical(aLabel, aEdge))
      {
        logC() << "Use " << aLabel << " to remove mesh elements from edge "
               << gm->getPhysicalString(aEdge) << std::endl;
        logC() << "Clear lines ..." << std::endl;
        aEdge->lines.clear();
      }
    }
    dt__forAllRefAuto(gm->dtFaces(), aFace)
    {
      if (gm->matchWildCardPhysical(aLabel, aFace))
      {
        logC() << "Use " << aLabel << " to remove mesh elements from face "
               << gm->getPhysicalString(aFace) << std::endl;
        logC() << "Clear triangles ..." << std::endl;
        aFace->triangles.clear();
        logC() << "Clear quadrangles ..." << std::endl;
        aFace->quadrangles.clear();
        logC() << "Clear polygons ..." << std::endl;
        aFace->polygons.clear();
        gm->untagPhysical(aFace);
      }
    }
  }
}
} // namespace dtOO
