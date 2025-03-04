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

#include "bVOWriteINP.h"

#include "bVOInterfaceFactory.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <constValueHeaven/constValue.h>
#include <logMe/dtParMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshModel.h>
#include <parseHeaven/dtParser.h>
#include <xmlHeaven/dtXmlParser.h>
#include <xmlHeaven/qtXmlBase.h>

namespace dtOO {
bool bVOWriteINP::_registrated =
  bVOInterfaceFactory::registrate(dt__tmpPtr(bVOWriteINP, new bVOWriteINP()));

bVOWriteINP::bVOWriteINP() {}

bVOWriteINP::~bVOWriteINP() {}

void bVOWriteINP::init(
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

  //		<bVObserver name="bVOWriteINP"
  //		  filename="mesh.inp"
  //		  saveAll="false"
  //		/>
  jsonPrimitive jE;
  jE.append<std::string>(
    "_filename", qtXmlBase::getAttributeStr("filename", element)
  );

  jE.append<bool>(
    "_saveAll", qtXmlBase::getAttributeBool("saveAll", element, false)
  );
  jE.append<bool>(
    "_saveGroupOfNodes",
    qtXmlBase::getAttributeBool("saveGroupOfNodes", element, false)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOWriteINP::postUpdate(void)
{
  dt__onlyMaster
  {
    if (!ptrBoundedVolume()->isMeshed())
    {
      dt__info(postUpdate(), << "Not yet meshed.");

      return;
    }

    dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

    //
    // set current model
    //
    ::GModel::setCurrent(gm);

    //
    // create filename string if empty
    //
    std::string cFileName =
      dtParser()[config().lookup<std::string>("_filename")];
    if (cFileName == "")
    {
      cFileName = dtXmlParser::constReference().currentState() + "_" +
                  ptrBoundedVolume()->getLabel() + ".inp";
    }
    //      else dt__throwUnexpected(postUpdate);

    dt__info(
      postUpdate(),
      << "Write >" << cFileName << "<." << std::endl
      << dt__eval(config().lookup<bool>("_saveAll")) << std::endl
      << dt__eval(config().lookup<bool>("_saveGroupOfNodes"))
    );

    ptrBoundedVolume()->getModel()->writeINP(
      cFileName,
      config().lookup<bool>("_saveAll"),
      config().lookup<bool>("_saveGroupOfNodes"),
      1.0
    );
  }
}
} // namespace dtOO
