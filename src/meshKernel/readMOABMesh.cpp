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

#include "readMOABMesh.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometryCompound.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <constValueHeaven/constValue.h>
#include <discrete3dPoints.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/stringPrimitive.h>
#include <logMe/logMe.h>
#include <unstructured3dMesh.h>

#include <iostream>
#include <moab/CN.hpp>
#include <moab/Core.hpp>

#include "boundedVolumeFactory.h"
#include <gmsh/MHexahedron.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MVertex.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>

namespace dtOO {
bool readMOABMesh::_registrated =
  boundedVolumeFactory::registrate(dt__tmpPtr(readMOABMesh, new readMOABMesh())
  );

readMOABMesh::readMOABMesh() { _mb.reset(new moab::Core()); }

readMOABMesh::~readMOABMesh() {}

void readMOABMesh::init(
  ::QDomElement const &element,
  baseContainer *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV
)
{
  //
  // init
  //
  gmshBoundedVolume::init(element, bC, cV, aF, aG, bV);

  _fileName = getOption("mesh_file");
}

void readMOABMesh::makeGrid(void)
{
  //
  // set current model
  //
  ::GModel::setCurrent(_gm);

  dt__throwIf(_mb == NULL, makeGrid());

  //
  // create error status
  //
  moab::ErrorCode rval;

  //
  // destroy mesh
  //
  rval = _mb->delete_mesh();
  moab__throwIf(rval != moab::MB_SUCCESS, makeGrid());

  //
  // load mesh
  //
  rval = _mb->load_mesh(_fileName.c_str());
  moab__throwIf(rval != moab::MB_SUCCESS, makeGrid());

  //
  // convert mesh to gmsh format
  //
  convertToGmsh();

  //
  // mark as meshed
  //
  boundedVolume::setMeshed();

  //
  // call observers
  //
  boundedVolume::postNotify();
}

void readMOABMesh::convertToGmsh(void)
{
  //
  // create type-string map
  //
  std::map<moab::EntityType, std::string> typeString;
  typeString[moab::MBVERTEX] = "moab::MBVERTEX";
  typeString[moab::MBEDGE] = "moab::MBEDGE";
  typeString[moab::MBTRI] = "moab::MBTRI";
  typeString[moab::MBQUAD] = "moab::MBQUAD";
  typeString[moab::MBPOLYGON] = "moab::MBPOLYGON";
  typeString[moab::MBTET] = "moab::MBTET";
  typeString[moab::MBPYRAMID] = "moab::MBPYRAMID";
  typeString[moab::MBPRISM] = "moab::MBPRISM";
  typeString[moab::MBKNIFE] = "moab::MBKNIFE";
  typeString[moab::MBHEX] = "moab::MBHEX";
  typeString[moab::MBPOLYHEDRON] = "moab::MBPOLYHEDRON";
  typeString[moab::MBENTITYSET] = "moab::MBENTITYSET";

  //
  // create error status
  //
  moab::ErrorCode rval;

  //
  // logContainer
  //
  logContainer<readMOABMesh> logC(logINFO, "getRender()");

  //
  // output
  //
  dt__forAllRefAuto(typeString, aTypeString)
  {
    moab::Range aRange;
    rval = _mb->get_entities_by_type(0, aTypeString.first, aRange);
    moab__throwIf(rval != moab::MB_SUCCESS, getRender());
    if (!aRange.empty())
    {
      logC() << aRange.size() << " elements of type " << aTypeString.second
             << std::endl;
    }
  }

  //
  // create vertices
  //
  _mv_MOAB = createVertices(_gm, *_mb);

  //
  // create faces
  //
  _ff_string = createFaces(_gm, *_mb, _mv_MOAB);

  //
  // create regions
  //
  _rr_string = createRegions(_gm, *_mb, _mv_MOAB);

  //
  // put all faces in first region
  //
  dt__forAllRefAuto(_ff_string, faceIt)
  {
    dtGmshFace *thisFace = faceIt.second;
    _rr_string.begin()->second->addFace(thisFace, 1);
    thisFace->addRegion(_rr_string.begin()->second);
  }

  //
  // put all vertices that are not on a face in the first region
  //
  dt__forAllRefAuto(_mv_MOAB, aPair)
  {
    ::MVertex *mv = aPair.second;
    if (mv->onWhat() == NULL)
    {
      mv->setEntity(_rr_string.begin()->second);
      _rr_string.begin()->second->addMeshVertex(mv);
    }
    else
    {
      mv->onWhat()->addMeshVertex(mv);
    }
  }
  //
  // create physicals
  //
  dt__forAllRefAuto(_ff_string, aPair)
  {
    _gm->tagPhysical(aPair.second, aPair.first);
  }
  dt__forAllRefAuto(_rr_string, aPair)
  {
    _gm->tagPhysical(aPair.second, aPair.first);
  }
}

std::map<::moab::EntityHandle, ::MVertex *>
readMOABMesh::createVertices(dtGmshModel *gm, moab::Interface const &mb)
{
  //
  // create error status
  //
  moab::ErrorCode rval;

  //
  // get vertices
  //
  moab::Range aRange;
  rval = mb.get_entities_by_type(0, moab::MBVERTEX, aRange);
  moab__throwIf(rval != moab::MB_SUCCESS, createVertices());

  //
  // store coordinates in vector
  //
  std::vector<double> xx(aRange.size());
  std::vector<double> yy(aRange.size());
  std::vector<double> zz(aRange.size());
  rval = mb.get_coords(aRange, &xx[0], &yy[0], &zz[0]);
  moab__throwIf(rval != moab::MB_SUCCESS, createVertices());

  //
  // create map
  //
  std::map<::moab::EntityHandle, ::MVertex *> mv_MOAB;
  dtInt ii = 0;
  for (moab::Range::iterator vIt = aRange.begin(); vIt != aRange.end(); vIt++)
  {
    moab::EntityHandle currentVertex = *vIt;

    mv_MOAB[currentVertex] = new ::MVertex(xx[ii], yy[ii], zz[ii]);
    ii++;
  }

  dt__info(createVertices(), << ii << " vertices created and mapped.");

  return mv_MOAB;
}

std::map<std::string, dtGmshFace *> readMOABMesh::createFaces(
  dtGmshModel *gm,
  moab::Interface const &mb,
  std::map<::moab::EntityHandle, ::MVertex *> &mv_MOAB
)
{
  //
  // create error status
  //
  moab::ErrorCode rval;

  //
  // logContainer
  //
  logContainer<readMOABMesh> logC(logINFO, "createFaces()");

  int nMeshSets;
  rval = mb.num_contained_meshsets(0, &nMeshSets, 0);
  moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

  logC() << logMe::dtFormat("[ x ] --- MeshSets ( %3i ) --- ") % nMeshSets
         << std::endl;

  moab::Range allSets;
  rval = mb.get_entities_by_type(0, moab::MBENTITYSET, allSets, true);
  moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

  dtInt meshSetCounter = 0;
  std::map<std::string, dtGmshFace *> ff_string;
  for (moab::Range::iterator it = allSets.begin(); it != allSets.end(); it++)
  {
    moab::EntityHandle currentSet = *it;

    //
    // get dimension of entities
    //
    std::vector<dtInt> dim(3, 0);
    rval = mb.get_number_entities_by_dimension(currentSet, 1, dim[0], true);
    moab__throwIf(rval != moab::MB_SUCCESS, createFaces());
    rval = mb.get_number_entities_by_dimension(currentSet, 2, dim[1], true);
    moab__throwIf(rval != moab::MB_SUCCESS, createFaces());
    rval = mb.get_number_entities_by_dimension(currentSet, 3, dim[2], true);
    moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

    //
    // only 2D elements
    //
    if (dim[1] == 0)
      continue;

    logC() << logMe::dtFormat("[ %i ] handle %i") % meshSetCounter %
                std::distance(allSets.begin(), it)
           << std::endl
           << logMe::dtFormat(
                "[ %i ] number of entities (1D, 2D, 3D) = (%i, %i, %i)"
              ) %
                meshSetCounter % dim[0] % dim[1] % dim[2]
           << std::endl;

    //
    // create new dtGmshFace
    //
    dtGmshFace *thisFace = new dtGmshFace(gm, gm->getMaxFaceTag() + 1);
    gm->add(thisFace);
    ff_string["meshSet_2d_" + stringPrimitive::intToString(meshSetCounter)] =
      thisFace;

    //
    // get quadrangles
    //
    moab::Range aRange;
    rval = mb.get_entities_by_type(currentSet, moab::MBQUAD, aRange);
    moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

    logC() << logMe::dtFormat("[ %i ] number of quadrangles %i") %
                meshSetCounter % aRange.size()
           << std::endl;

    //
    // create MQuadrangle
    //
    for (moab::Range::iterator it = aRange.begin(); it != aRange.end(); it++)
    {
      //
      // connectivities
      //
      moab::EntityHandle const *conn;
      dtInt nNodes = 0;
      rval = mb.get_connectivity(*it, conn, nNodes);
      moab__throwIf(rval != moab::MB_SUCCESS, createFaces());

      dt__throwIf(nNodes != 4, createFaces());
      dt__forFromToIndex(0, nNodes, ii)
      {
        if (mv_MOAB[conn[ii]] == NULL)
        {
          dt__throw(
            createFaces(),
            << "Element " << std::distance(aRange.begin(), it)
            << " contains conn[ " << ii << " ] = " << (unsigned int)conn[ii]
            << std::endl
            << "mv_MOAB[ " << conn[ii] << " ] = " << mv_MOAB[conn[ii]]
          );
        }
      }

      thisFace->addQuadrangle(new ::MQuadrangle(
        mv_MOAB[conn[0]], mv_MOAB[conn[1]], mv_MOAB[conn[2]], mv_MOAB[conn[3]]
      ));

      //
      // add MVertex to this GFace
      // - if another element on another face also contains one of these
      //   vertices then the owner (face) will change.
      //
      mv_MOAB[conn[0]]->setEntity(thisFace);
      mv_MOAB[conn[1]]->setEntity(thisFace);
      mv_MOAB[conn[2]]->setEntity(thisFace);
      mv_MOAB[conn[3]]->setEntity(thisFace);
    }
    meshSetCounter++;
  }

  return ff_string;
}

std::map<std::string, dtGmshRegion *> readMOABMesh::createRegions(
  dtGmshModel *gm,
  moab::Interface const &mb,
  std::map<::moab::EntityHandle, ::MVertex *> &mv_MOAB
)
{
  //
  // create error status
  //
  moab::ErrorCode rval;

  //
  // logContainer
  //
  logContainer<readMOABMesh> logC(logINFO, "createRegions()");

  int nMeshSets;
  rval = mb.num_contained_meshsets(0, &nMeshSets, 0);
  moab__throwIf(rval != moab::MB_SUCCESS, createRegions());

  logC() << logMe::dtFormat("[ x ] --- MeshSets ( %3i ) --- ") % nMeshSets
         << std::endl;

  moab::Range allSets;
  rval = mb.get_entities_by_type(0, moab::MBENTITYSET, allSets, true);
  moab__throwIf(rval != moab::MB_SUCCESS, createRegions());

  dtInt meshSetCounter = 0;
  std::map<std::string, dtGmshRegion *> rr_string;
  for (moab::Range::iterator it = allSets.begin(); it != allSets.end(); it++)
  {
    moab::EntityHandle currentSet = *it;

    //
    // get dimension of entities
    //
    std::vector<dtInt> dim(3, 0);
    rval = mb.get_number_entities_by_dimension(currentSet, 1, dim[0], true);
    moab__throwIf(rval != moab::MB_SUCCESS, createRegions());
    rval = mb.get_number_entities_by_dimension(currentSet, 2, dim[1], true);
    moab__throwIf(rval != moab::MB_SUCCESS, createRegions());
    rval = mb.get_number_entities_by_dimension(currentSet, 3, dim[2], true);
    moab__throwIf(rval != moab::MB_SUCCESS, createRegions());

    //
    // only 3D elements
    //
    if (dim[2] == 0)
      continue;

    logC() << logMe::dtFormat("[ %i ] handle %i") % meshSetCounter % currentSet
           << std::endl
           << logMe::dtFormat(
                "[ %i ] number of entities (1D, 2D, 3D) = (%i, %i, %i)"
              ) %
                meshSetCounter % dim[0] % dim[1] % dim[2]
           << std::endl;

    //
    // create dtGmshRegion
    //
    dtGmshRegion *thisRegion = new dtGmshRegion(gm, gm->getMaxRegionTag() + 1);
    gm->add(thisRegion);
    rr_string["meshSet_3d_" + stringPrimitive::intToString(meshSetCounter)] =
      thisRegion;

    //
    // get hexahedrons
    //
    moab::Range aRange;
    rval = mb.get_entities_by_type(currentSet, moab::MBHEX, aRange);
    moab__throwIf(rval != moab::MB_SUCCESS, createRegions());

    logC() << logMe::dtFormat("[ %i ] number of hexahedra %i") %
                meshSetCounter % aRange.size()
           << std::endl;

    //
    // create MHexahedron
    //
    for (moab::Range::iterator it = aRange.begin(); it != aRange.end(); it++)
    {
      //
      // connectivities
      //
      moab::EntityHandle const *conn;
      dtInt nNodes;
      rval = mb.get_connectivity(*it, conn, nNodes);
      moab__throwIf(rval != moab::MB_SUCCESS, createRegions());

      thisRegion->addHexahedron(new ::MHexahedron(
        mv_MOAB[conn[0]],
        mv_MOAB[conn[1]],
        mv_MOAB[conn[2]],
        mv_MOAB[conn[3]],
        mv_MOAB[conn[4]],
        mv_MOAB[conn[5]],
        mv_MOAB[conn[6]],
        mv_MOAB[conn[7]]
      ));
    }
    meshSetCounter++;
  }

  return rr_string;
}
} // namespace dtOO
