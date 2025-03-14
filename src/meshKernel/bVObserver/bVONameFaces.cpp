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

#include "bVONameFaces.h"

#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <vector>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVONameFaces::_registrated =
  bVOInterfaceFactory::registrate(dt__tmpPtr(bVONameFaces, new bVONameFaces()));

bVONameFaces::bVONameFaces() {}

bVONameFaces::~bVONameFaces() {}

void bVONameFaces::init(
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
  //   name="bVONameFaces"
  //   faceLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
  // />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::vector<std::string>>(
    "_faceLabel", dtXmlParserBase::getAttributeStrVector("faceLabel", element)
  );
  jE.append<std::vector<std::string>>(
    "_face", dtXmlParserBase::getAttributeStrVector("face", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVONameFaces::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  logContainer<bVONameFaces> logC(logINFO, "preUpdate()");

  std::vector<std::string> faceLabel =
    this->config().lookup<std::vector<std::string>>("_faceLabel");
  std::vector<std::string> face =
    this->config().lookup<std::vector<std::string>>("_face");

  if (faceLabel.empty())
  {
    logC() << "Apply automatic naming" << std::endl;
    dt__forAllRefAuto(gm->regions(), aReg)
    {
      dtInt cc = 0;
      logC() << "Region : " << gm->getPhysicalString(aReg) << std::endl;
      dt__forAllRefAuto(aReg->faces(), aFace)
      {
        if (gm->getPhysicalString(aFace) == "")
        {
          gm->tagPhysical(
            aFace,
            gm->getPhysicalString(aReg) + "_F_" +
              stringPrimitive::intToString(cc)
          );
        }
        logC() << "  Face " << cc << " : " << gm->getPhysicalString(aFace)
               << std::endl;
        cc++;
      }
    }
  }
  else
  {
    if (face.size() == faceLabel.size())
    {
      // int cc = 0;
      dt__forAllIndex(face, cc)
      {
        dtGmshFace *gf = gm->getDtGmshFaceByPhysical(face[cc]);
        gm->tagPhysical(gf, faceLabel[cc]);
        logC() << logMe::dtFormat("Tag face : %s -> %s") % face[cc] %
                    faceLabel[cc]
               << std::endl;
      }
    }
    else
    {
      dtInt counter = 0;
      dt__forAllRefAuto(gm->faces(), aFace)
      {
        std::vector<dtInt> pInt = aFace->getPhysicalEntities();
        dt__throwIf(pInt.size() != 0, preUpdate());

        std::string newL = faceLabel[counter];
        if (newL != "")
        {
          dtInt pTag = aFace->model()->setPhysicalName(newL, 2, 0);
          aFace->addPhysicalEntity(pTag);
          dtGmshModel::intGEntityVMap map;
          gm->getPhysicalGroups(2, map);
          logC() << logMe::dtFormat(
                      "Physical group %d / %s ( %d faces ) -> add face %d"
                    ) %
                      newL % pTag % map[pTag].size() % aFace->tag()
                 << std::endl;
        }
        counter++;
      }
    }
  }
}
} // namespace dtOO
