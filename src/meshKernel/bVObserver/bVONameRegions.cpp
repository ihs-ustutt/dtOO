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

#include "bVONameRegions.h"

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
bool bVONameRegions::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVONameRegions, new bVONameRegions())
);

bVONameRegions::bVONameRegions() {}

bVONameRegions::~bVONameRegions() {}

void bVONameRegions::init(
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
  //   name="bVONameRegions"
  //   regionLabel="{name0}"
  // />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::vector<std::string>>(
    "_regionLabel",
    dtXmlParserBase::getAttributeStrVector("regionLabel", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVONameRegions::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  //
  // check size
  //
  std::vector<std::string> regionLabel =
    this->config().lookup<std::vector<std::string>>("_regionLabel");
  dt__throwIf(
    !regionLabel.empty() && (regionLabel.size() != gm->getNumRegions()),
    preUpdate()
  );

  logContainer<bVONameRegions> logC(logINFO, "preUpdate()");
  if (regionLabel.empty())
  {
    logC() << "Apply automatic naming" << std::endl;
    dtInt cc = 0;
    dt__forAllRefAuto(gm->regions(), aReg)
    {
      if (gm->getPhysicalString(aReg) == "")
      {
        gm->tagPhysical(aReg, "R_" + stringPrimitive::intToString(cc));
      }
      logC() << "Region : " << gm->getPhysicalString(aReg) << std::endl;
      cc++;
    }
  }
  else
  {
    dtInt counter = 0;
    dt__forAllRefAuto(gm->regions(), aReg)
    {
      std::vector<dtInt> pInt = aReg->getPhysicalEntities();
      dt__throwIf(pInt.size() != 0, preUpdate());

      std::string newL = regionLabel[counter];

      if (newL != "")
      {
        dtInt pTag = aReg->model()->setPhysicalName(newL, 3, 0);
        aReg->addPhysicalEntity(pTag);
        dtGmshModel::intGEntityVMap map;
        gm->getPhysicalGroups(3, map);
        logC() << logMe::dtFormat(
                    "Physical group %d / %s ( %d regions ) -> add region %d"
                  ) %
                    newL % pTag % map[pTag].size() % aReg->tag()
               << std::endl;
      }
      counter++;
    }
  }
}
} // namespace dtOO
