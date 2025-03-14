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

#include "bVOSetNElements.h"

#include "bVOInterfaceFactory.h"
#include "jsonHeaven/jsonPrimitive.h"
#include <boundedVolume.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVOSetNElements::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOSetNElements, new bVOSetNElements())
);

bVOSetNElements::bVOSetNElements() {}

bVOSetNElements::~bVOSetNElements() {}

void bVOSetNElements::init(
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

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::vector<std::string>>(
    "_faceLabel",
    dtXmlParserBase::getAttributeStrVector(
      "faceLabel", element, std::vector<std::string>()
    )
  );

  jE.append<std::vector<std::string>>(
    "_regionLabel",
    dtXmlParserBase::getAttributeStrVector(
      "regionLabel", element, std::vector<std::string>()
    )
  );
  jE.append<std::vector<dtInt>>(
    "_nE",
    dtXmlParserBase::getAttributeIntVectorMuParse(
      "numberElements", element, cV, aF
    )
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOSetNElements::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  std::vector<dtInt> nE = config().lookup<std::vector<dtInt>>("_nE");
  dt__forAllRefAuto(
    config().lookup<std::vector<std::string>>("_faceLabel"), aLabel
  )
  {
    if (nE.size() == 2)
    {
      dt__forAllRefAuto(gm->getDtGmshFaceListByPhysical(aLabel), aFace)
      {
        aFace->meshWNElements(nE[0], nE[1]);
      }
    }
    else
    {
      dt__forAllRefAuto(gm->getDtGmshFaceListByPhysical(aLabel), aFace)
      {
        aFace->meshWNElements(nE[0], nE[1], nE[2], nE[3]);
      }
    }
  }
  dt__forAllRefAuto(
    config().lookup<std::vector<std::string>>("_regionLabel"), aLabel
  )
  {
    if (nE.size() == 3)
    {
      dt__forAllRefAuto(gm->getDtGmshRegionListByPhysical(aLabel), aReg)
      {
        aReg->meshWNElements(nE[0], nE[1], nE[2]);
      }
    }
  }
}
} // namespace dtOO
