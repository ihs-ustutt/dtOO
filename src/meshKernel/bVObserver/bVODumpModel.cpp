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

#include "bVODumpModel.h"

#include <interfaceHeaven/ptrHandling.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <logMe/logMe.h>
#include <vector>
// #include <math.h>
#include "bVOInterfaceFactory.h"
#include <boundedVolume.h>
#include <gmsh/Context.h>
#include <gmsh/Field.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshVertex.h>
#include <xmlHeaven/dtXmlParserBase.h>

namespace dtOO {
bool bVODumpModel::_registrated =
  bVOInterfaceFactory::registrate(dt__tmpPtr(bVODumpModel, new bVODumpModel()));

bVODumpModel::bVODumpModel() {}

bVODumpModel::~bVODumpModel() {}

void bVODumpModel::preUpdate(void)
{
  dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());

  //
  // set current model
  //
  ::GModel::setCurrent(gm);

  //
  // check size
  //
  logContainer<bVODumpModel> logC(logINFO, "preUpdate()");
  logC() << "number GVertex = " << gm->getNumVertices() << std::endl
         << "number GEdge = " << gm->getNumEdges() << std::endl
         << "number GFace = " << gm->getNumFaces() << std::endl
         << "number GRegion = " << gm->getNumRegions() << std::endl;
  dt__forFromToIndex(0, 4, dim)
  {
    dtGmshModel::intGEntityVMap map;
    gm->getPhysicalGroups(dim, map);
    dt__forAllRefAuto(map, aPair)
    {
      logC() << logMe::dtFormat(
                  "Physical group ( %d ): name = %s, dim = %d ( %d entities )"
                ) %
                  aPair.first % gm->getPhysicalName(dim, aPair.first) % dim %
                  aPair.second.size()
             << std::endl;
    }
  }
  dt__forAllRefAuto(gm->vertices(), aV)
  {
    if (!aV)
    {
      logC() << "dtGmshVertex[] = NULL" << std::endl;
      continue;
    }
    logC() << logMe::dtFormat("dtGmshVertex[ %3i ] = %s ( %x )") % aV->tag() %
                gm->getPhysicalString(aV) % aV
           << std::endl
           << "  edges = " << aV->edges() << std::endl;
  }
  dt__forAllRefAuto(gm->edges(), aE)
  {
    if (!aE)
    {
      logC() << "dtGmshEdge[] = NULL" << std::endl;
      continue;
    }
    logC() << logMe::dtFormat("dtGmshEdge[ %3i ] = %s ( %x )") % aE->tag() %
                gm->getPhysicalString(aE) % aE
           << std::endl
           << "  faces = " << aE->faces() << std::endl
           << "  vertices = " << aE->vertices() << std::endl
           << "  fullPhysicalList = [" << std::endl;
    dt__forAllRefAuto(gm->getFullPhysicalList(aE), aString)
    {
      logC() << aString << std::endl;
    }
    logC() << "]" << std::endl;
  }
  dt__forAllRefAuto(gm->faces(), aF)
  {
    if (!aF)
    {
      logC() << "dtGmshFace[] = NULL" << std::endl;
      continue;
    }
    logC() << logMe::dtFormat("dtGmshFace[ %3i ] = %s ( %x )") % aF->tag() %
                gm->getPhysicalString(aF) % aF
           << std::endl
           << "  edges = " << aF->edges() << std::endl
           << "  edgeOrientations = " << aF->edgeOrientations() << std::endl
           << "  regions = " << aF->regions() << std::endl
           << "  fullPhysicalList = [" << std::endl;
    dt__forAllRefAuto(gm->getFullPhysicalList(aF), aString)
    {
      logC() << aString << std::endl;
    }
    logC() << "]" << std::endl;
  }
  dt__forAllRefAuto(gm->regions(), aR)
  {
    if (!aR)
    {
      logC() << "dtGmshRegion[] = NULL" << std::endl;
      continue;
    }
    logC() << logMe::dtFormat("dtGmshRegion[ %3i ] = %s ( %x )") % aR->tag() %
                gm->getPhysicalString(aR) % aR
           << std::endl
           << "  faceList = " << aR->faces() << std::endl
           << "  faceOrientations = " << aR->faceOrientations() << std::endl
           << "  fullPhysicalList = [" << std::endl;
    dt__forAllRefAuto(gm->getFullPhysicalList(aR), aString)
    {
      logC() << aString << std::endl;
    }
    logC() << "]" << std::endl;
  }
  logC() << "Gmsh fieldManager:" << std::endl
         << "size: " << gm->getFields()->size() << std::endl
         << dt__eval(CTX::instance()->mesh.lcFromPoints) << std::endl
         << dt__eval(CTX::instance()->mesh.lcFromCurvature) << std::endl
         << dt__eval(CTX::instance()->mesh.lcExtendFromBoundary) << std::endl;
}

void bVODumpModel::postUpdate(void) { preUpdate(); }
} // namespace dtOO
