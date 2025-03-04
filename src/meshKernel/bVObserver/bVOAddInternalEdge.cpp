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

#include "bVOAddInternalEdge.h"
#include "analyticGeometryHeaven/map1dTo3d.h"

#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <boundedVolume.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <xmlHeaven/dtXmlParserBase.h>

#include "bVOInterfaceFactory.h"
#include <gmsh/GEdgeLoop.h>
#include <progHelper.h>

namespace dtOO {
bool bVOAddInternalEdge::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVOAddInternalEdge, new bVOAddInternalEdge())
);

bVOAddInternalEdge::bVOAddInternalEdge() {}

bVOAddInternalEdge::~bVOAddInternalEdge() {}

void bVOAddInternalEdge::init(
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
  //   name="bVOAddInternalEdge"
  //   regionLabel="name0"
  // />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  jsonPrimitive jE;
  jE.append<std::string>(
    "_regionLabel", dtXmlParserBase::getAttributeStr("regionLabel", element)
  );
  bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
}

void bVOAddInternalEdge::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  dtGmshRegion *const gr =
    gm->getDtGmshRegionByPhysical(config().lookup<std::string>("_regionLabel"));

  std::vector<dtGmshFace *> fV = progHelper::list2Vector(gr->dtFaces());

  std::vector<GEdge *> e0;
  std::vector<GEdge *> e2;
  dt__forFromToIndex(6, fV.size(), ii)
  {
    std::vector<::GEdge *> eV = fV[ii]->edges();
    dt__forAllRefAuto(eV, anEdge)
    {
      if (fV[0]->isOnFace(anEdge))
        e0.push_back(anEdge);
      if (fV[1]->isOnFace(anEdge))
        e2.push_back(anEdge);
    }
  }

  dt__debug(
    preUpdate(),
    << e0.size() << " edges lie in fV[0]." << std::endl
    << e2.size() << " edges lie in fV[1]."
  );

  ::GEdgeLoop el0(e0);
  for (::GEdgeLoop::citer it = el0.begin(); it != el0.end(); ++it)
  {
    fV[0]->addEdge(it->getEdge(), it->getSign());
  }
  ::GEdgeLoop el2(e2);
  for (::GEdgeLoop::citer it = el2.begin(); it != el2.end(); ++it)
  {
    fV[1]->addEdge(it->getEdge(), it->getSign());
  }
}
} // namespace dtOO
