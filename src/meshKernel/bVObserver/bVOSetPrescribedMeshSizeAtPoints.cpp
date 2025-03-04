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

#include "bVOSetPrescribedMeshSizeAtPoints.h"

#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <math.h>
#include <meshEngine/dtGmshModel.h>
#include <vector>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVOSetPrescribedMeshSizeAtPoints::_registrated =
  bVOInterfaceFactory::registrate(dt__tmpPtr(
    bVOSetPrescribedMeshSizeAtPoints, new bVOSetPrescribedMeshSizeAtPoints()
  ));

bVOSetPrescribedMeshSizeAtPoints::bVOSetPrescribedMeshSizeAtPoints() {}

bVOSetPrescribedMeshSizeAtPoints::~bVOSetPrescribedMeshSizeAtPoints() {}

void bVOSetPrescribedMeshSizeAtPoints::init(
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
  //   name="bVOSetPrescribedMeshSizeAtPoints"
  //   meshSize="0.1"
  // />

  jsonPrimitive jE;
  if (dtXmlParserBase::isAttributeVector("meshSize", element))
  {
    jE.append<std::vector<dtReal>>(
      "_meshSize",
      dtXmlParserBase::getAttributeFloatVectorMuParse(
        "meshSize", element, cV, aF
      )
    );
  }
  else
  {
    jE.append<std::vector<dtReal>>(
      "_meshSize",
      std::vector<dtReal>(
        1,
        dtXmlParserBase::getAttributeFloatMuParse("meshSize", element, cV, aF)
      )
    );
  }
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOSetPrescribedMeshSizeAtPoints::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);
  std::vector<dtReal> meshSize =
    config().lookup<std::vector<dtReal>>("_meshSize");
  if (meshSize.size() == 1)
  {
    meshSize = std::vector<dtReal>(gm->getNumVertices(), meshSize[0]);
  }
  dt__throwIf(gm->getNumVertices() != meshSize.size(), preUpdate());

  dtInt cc = 0;
  dt__forAllRefAuto(gm->vertices(), aV)
  {
    aV->setPrescribedMeshSizeAtVertex(meshSize[cc]);
    cc = cc + 1;
  }

  //
  // output
  //
  logContainer<bVOSetPrescribedMeshSizeAtPoints> logC(logINFO, "preUpdate()");
  dt__forAllRefAuto(gm->vertices(), aV)
  {
    logC() << logMe::dtFormat("dtGmshVertex[ %3i ]: meshSize = %f") %
                aV->tag() % aV->prescribedMeshSizeAtVertex()
           << std::endl;
  }
}
} // namespace dtOO
