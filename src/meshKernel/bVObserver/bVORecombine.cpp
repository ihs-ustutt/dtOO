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

#include "bVORecombine.h"

#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVORecombine::_registrated =
  bVOInterfaceFactory::registrate(dt__tmpPtr(bVORecombine, new bVORecombine()));

bVORecombine::bVORecombine() {}

bVORecombine::~bVORecombine() {}

void bVORecombine::init(
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
  //   name="bVORecombine"
  //   regionLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
  // />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::vector<std::string>>(
    "_faceLabel", dtXmlParserBase::getAttributeStrVector("faceLabel", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVORecombine::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  dt__forAllRefAuto(
    config().lookup<std::vector<std::string>>("_faceLabel"), aLabel
  )
  {
    dt__forAllRefAuto(gm->getDtGmshFaceListByPhysical(aLabel), aFace)
    {
      aFace->meshRecombine();
    }
  }
}
} // namespace dtOO
