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

#include "bVOTransfiniteFaces.h"

#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVOTransfiniteFaces::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOTransfiniteFaces, new bVOTransfiniteFaces())
);

bVOTransfiniteFaces::bVOTransfiniteFaces() {}

bVOTransfiniteFaces::~bVOTransfiniteFaces() {}

void bVOTransfiniteFaces::init(
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
  //   name="bVOTransfiniteFaces"
  //   faceLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
  //   numberElements="{numU}{numV}"
  // />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::vector<std::string>>(
    "_faceLabel", dtXmlParserBase::getAttributeStrVector("faceLabel", element)
  );
  jE.append<std::vector<dtInt>>(
    "_nE",
    dtXmlParserBase::getAttributeIntVectorMuParse(
      "numberElements", element, cV, aF, aG
    )
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOTransfiniteFaces::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  std::vector<dtInt> nE = config().lookup<std::vector<dtInt>>("_nE");
  dt__throwIf(nE.size() != 2, preUpdate());

  dt__forAllRefAuto(
    config().lookup<std::vector<std::string>>("_faceLabel"), aLabel
  )
  {
    dt__forAllRefAuto(gm->getDtGmshFaceListByPhysical(aLabel), aFace)
    {
      aFace->meshTransfinite();
      aFace->meshWNElements(nE[0], nE[1]);
    }
  }
  dt__forAllRefAuto(gm->faces(), aFace)
  {
    dtGmshModel::cast2DtGmshFace(aFace)->correctIfTransfinite();
  }
}
} // namespace dtOO
