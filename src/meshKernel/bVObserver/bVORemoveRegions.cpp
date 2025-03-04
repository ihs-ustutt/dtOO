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

#include "bVORemoveRegions.h"

#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshVertex.h>

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
bool bVORemoveRegions::_registrated = bVOInterfaceFactory::registrate(
  dt__tmpPtr(bVORemoveRegions, new bVORemoveRegions())
);

bVORemoveRegions::bVORemoveRegions() {}

bVORemoveRegions::~bVORemoveRegions() {}

void bVORemoveRegions::init(
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
  //   name="bVORemoveRegions"
  //   regionLabel="{name0}"
  // />

  dt__info(init(), << dtXmlParserBase::convertToString(element));
  _regionLabel = dtXmlParserBase::getAttributeStrVector("regionLabel", element);
}

void bVORemoveRegions::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  logContainer<bVORemoveRegions> logC(logINFO, "preUpdate()");
  dt__forAllRefAuto(_regionLabel, aLabel)
  {
    std::list<dtGmshRegion *> regL = gm->getDtGmshRegionListByPhysical(aLabel);
    dt__forAllRefAuto(regL, aReg)
    {
      //
      // remove region on faces
      //
      dt__forAllRefAuto(aReg->dtFaces(), aFace) aFace->delRegion(aReg);
      //
      // remove region in model
      //
      gm->remove(aReg);
    }
  }
  //
  // remove all faces that do not belong to at least one region
  //
  dt__forAllRefAuto(gm->dtFaces(), aFace)
  {
    if (aFace->dtRegions().empty())
    {
      //
      // delte face from all edges
      //
      dt__forAllRefAuto(aFace->dtEdges(), aEdge) aEdge->delFace(aFace);
      gm->remove(aFace);
    }
  }
  //
  // remove all edges that do not belong to at least one face
  //
  dt__forAllRefAuto(gm->dtEdges(), aEdge)
  {
    if (aEdge->dtFaces().empty())
    {
      //
      // delte face from all edges
      //
      dt__forAllRefAuto(aEdge->dtVertices(), aVertex) aVertex->delEdge(aEdge);
      gm->remove(aEdge);
    }
  }
  //
  // remove all vertices that do not belong to at least one edge
  //
  dt__forAllRefAuto(gm->dtVertices(), aVertex)
  {
    if (aVertex->dtEdges().empty())
    {
      //
      // delte face from all edges
      //
      gm->remove(aVertex);
    }
  }
}
} // namespace dtOO
