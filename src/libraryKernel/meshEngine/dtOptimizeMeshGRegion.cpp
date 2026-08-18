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

#include "dtOptimizeMeshGRegion.h"

#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include "dtMeshOperatorFactory.h"
#include <gmsh/MTetrahedron.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MVertex.h>
#include <gmsh/meshGRegion.h>
#include <gmsh/meshRelocateVertex.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
bool dtOptimizeMeshGRegion::_registrated = dtMeshOperatorFactory::registrate(
  dt__tmpPtr(dtOptimizeMeshGRegion, new dtOptimizeMeshGRegion())
);

dtOptimizeMeshGRegion::dtOptimizeMeshGRegion() : dtMesh3DOperator() {}

dtOptimizeMeshGRegion::~dtOptimizeMeshGRegion() {}

bool dtOptimizeMeshGRegion::isStatusIndependent(void) { return true; }

dtOptimizeMeshGRegion::dtOptimizeMeshGRegion(const dtOptimizeMeshGRegion &orig)
  : dtMesh3DOperator(orig)
{
}

void dtOptimizeMeshGRegion::operator()(dtGmshRegion *dtgr)
{
  Msg::Info("dtOptimizeMeshGRegion()() volume %d", dtgr->tag());
  if (config().lookupDef<bool>("_relocateVertices", false))
  {
    Msg::Info(
      "dtOptimizeMeshGRegion()() volume %d : RelocateVertices", dtgr->tag()
    );
    RelocateVertices(dtgr, config().lookup<int>("_relocateVerticesNumIter"));
  }

  if (config().lookupDef<bool>("_relocateVerticesOfPyramids", false))
  {
    Msg::Info(
      "dtOptimizeMeshGRegion()() volume %d : RelocateVerticesOfPyramids",
      dtgr->tag()
    );
    RelocateVerticesOfPyramids(
      dtgr, config().lookup<int>("_relocateVerticesOfPyramidsNumIter")
    );
  }
  if (config().lookupDef<bool>("_gmsh", true))
  {
    Msg::Info(
      "dtOptimizeMeshGRegion()() volume %d : Optimize gmsh", dtgr->tag()
    );
    ::optimizeMeshGRegion()(dtgr);
  }

  if (config().lookupDef<bool>("_netgen", true))
  {
    Msg::Info(
      "dtOptimizeMeshGRegion()() volume %d : Optimize netgen", dtgr->tag()
    );
    this->optimizeNetgen(dtgr);
  }
}

void dtOptimizeMeshGRegion::optimizeNetgen(dtGmshRegion *dtgr) const
{
  //
  // create pseudoRegion and pseudoFace; pseudoFace contains all tetrahedra
  // faces with no counterpart
  //
  dtGmshRegion *pseudoRegionPtr = new dtGmshRegion(
    dtgr->model(), dtgr->refDtGmshModel().getMaxRegionTag() + 1
  );
  dtGmshFace *pseudoFacePtr =
    new dtGmshFace(dtgr->model(), dtgr->refDtGmshModel().getMaxFaceTag() + 1);
  dtGmshRegion &pseudoRegion = *pseudoRegionPtr;
  dtGmshFace &pseudoFace = *pseudoFacePtr;

  //
  // add region to model
  //
  pseudoRegion.addGEntity(&pseudoFace);
  dtgr->model()->add(&pseudoRegion);

  //
  // extract mesh vertices from tetrahedrons
  //
  std::vector<::MVertex *> tet_verts = extractVerts(dtgr->tetrahedra);

  //
  // clone mesh vertices of tetrahedrons
  //
  std::map<::MVertex *, ::MVertex *> clone_org;
  std::map<::MVertex *, ::MVertex *> org_clone;
  dt__forAllRefAuto(tet_verts, aVert)
  {
    clone_org[aVert] = new ::MVertex(aVert->x(), aVert->y(), aVert->z(), NULL);
    org_clone[clone_org[aVert]] = aVert;
  }

  //
  // sort original tetrahedra deterministically before creating the
  // pseudoRegion. The original vertices are used here, so getNum() is
  // directly available.
  //
  sortTetrahedraDeterministically(dtgr->tetrahedra, org_clone);

  //
  // extract coupling faces pyramids/tetrahedra and prisms/tetrahedra
  //
  Msg::Info("Extract outter faces of tetrahedra");
  std::vector<::MTriangle *> single_faces =
    extractSingleFaces(dtgr->tetrahedra);
  //
  // add coupling faces to cloneFaceElem
  //
  dt__forAllRefAuto(single_faces, aTri) { pseudoFace.addElement(aTri); }

  //
  // copy all tetrahedrons
  //
  dt__forAllRefAuto(dtgr->tetrahedra, aTet)
  {
    MTetrahedron *aTetCopy = new ::MTetrahedron(
      aTet->getVertex(0),
      aTet->getVertex(1),
      aTet->getVertex(2),
      aTet->getVertex(3)
    );
    pseudoRegion.addTetrahedron(aTetCopy);
  }

  //
  // replace old vertices in triangles and tetrahedra with new cloned vertices
  //
  dt__forAllRefAuto(pseudoFace.triangles, aTri)
  {
    dt__forFromToIndex(0, aTri->getNumVertices(), vertIndex)
    {
      ::MVertex *org = aTri->getVertex(vertIndex);
      auto it_clone = clone_org.find(org);
      dt__throwIfWithMessage(
        it_clone == clone_org.end(),
        optimizeNetgen(),
        << "Vertex on pseudoFace was not cloned."
      );
      aTri->setVertex(vertIndex, it_clone->second);
    }
  }
  dt__forAllRefAuto(pseudoRegion.tetrahedra, aTet)
  {
    dt__forFromToIndex(0, aTet->getNumVertices(), vertIndex)
    {
      ::MVertex *org = aTet->getVertex(vertIndex);
      auto it_clone = clone_org.find(org);
      dt__throwIfWithMessage(
        it_clone == clone_org.end(),
        optimizeNetgen(),
        << "Vertex in pseudoRegion was not cloned."
      );
      aTet->setVertex(vertIndex, it_clone->second);
    }
  }

  //
  // set clone vertices to correct new clone entity
  //
  dt__forAllRefAuto(pseudoRegion.tetrahedra, aTet)
  {
    dt__forFromToIndex(0, aTet->getNumVertices(), vertIndex)
    {
      ::MVertex *org = aTet->getVertex(vertIndex);
      if (org->onWhat() == NULL)
      {
        org->setEntity(&pseudoRegion);
        pseudoRegion.addMeshVertex(org);
      }
    }
  }
  dt__forAllRefAuto(pseudoFace.triangles, aTri)
  {
    dt__forFromToIndex(0, aTri->getNumVertices(), vertIndex)
    {
      ::MVertex *org = aTri->getVertex(vertIndex);
      if (org->onWhat() == &pseudoRegion)
      {
        pseudoRegion.removeMeshVertex(org);
        org->setEntity(&pseudoFace);
        pseudoFace.addMeshVertex(org);
      }
    }
  }

  //
  // do optimization
  //
  ::optimizeMeshGRegionNetgen()(&pseudoRegion);

  //
  // restore deterministic tetrahedron ordering after Netgen
  //
  sortTetrahedraDeterministically(pseudoRegion.tetrahedra, org_clone);

  //
  // destroy old tetrahedrons
  //
  dt__forAllRefAuto(dtgr->tetrahedra, aTet) { delete aTet; }
  dtgr->tetrahedra.clear();

  //
  // remove mesh vertices cloned in dtgr region
  //
  dt__forAllRefAuto(org_clone, aPair)
  {
    // if (aPair.second->onWhat() == dtgr)
    if (aPair.first->onWhat() == &pseudoRegion)
    {
      dtgr->removeMeshVertex(aPair.second, true);
    }
  }

  // int counter = 0;
  dt__forAllIndex(pseudoRegion.tetrahedra, ii)
  {
    ::MTetrahedron *aTet = pseudoRegion.tetrahedra[ii];
    dtgr->tetrahedra.push_back(aTet);
    dt__forFromToIndex(0, aTet->getNumVertices(), vertIndex)
    {
      // move vertex to dtgr
      ::MVertex *vert = aTet->getVertex(vertIndex);
      if (vert->onWhat() == &pseudoRegion)
      {
        dtgr->addMeshVertex(vert);
        vert->setEntity(dtgr);
        pseudoRegion.removeMeshVertex(vert);
      }
      // vertex is still in dtgr, it was a coupling faces vertex
      // vertex should not be deleted at this point
      else if (vert->onWhat() == &pseudoFace)
      {
        aTet->setVertex(vertIndex, org_clone[vert]);
        pseudoFace.removeMeshVertex(vert, false);
      }
    }
  }
  // only clear tetrahedra vector, because the tet were moved to the dtgr
  // region
  pseudoRegion.tetrahedra.clear();

  //
  // clear model
  //
  pseudoRegion.deleteFace(&pseudoFace);
  pseudoFace.delRegion(&pseudoRegion);
  dtgr->model()->remove(&pseudoRegion);
  dtgr->model()->remove(&pseudoFace);
}

template <typename T>
std::vector<::MVertex *>
dtOptimizeMeshGRegion::extractVerts(std::vector<T *> elems) const
{
  std::vector<::MVertex *> verts;
  dt__forAllRefAuto(elems, anElem)
  {
    dt__forFromToIndex(0, anElem->getNumVertices(), vertIndex)
    {
      MVertex *aVert = anElem->getVertex(vertIndex);
      verts.push_back(aVert);
    }
  }
  // sort by deterministic MVertex number instead of pointer address
  std::sort(verts.begin(), verts.end(), MVertexPtrLessThan());
  verts.erase(unique(verts.begin(), verts.end()), verts.end());
  return verts;
}

template std::vector<::MVertex *>
dtOptimizeMeshGRegion::extractVerts(std::vector<::MTetrahedron *> elems) const;

std::vector<::MTriangle *>
dtOptimizeMeshGRegion::extractSingleFaces(std::vector<::MTetrahedron *> tets
) const
{
  int faceCounter = 0;

  //
  // Compare face keys using MVertex::getNum(), not pointer addresses.
  //
  struct MVertexVectorPtrLessThan {
    bool operator()(
      const std::vector<::MVertex *> &a, const std::vector<::MVertex *> &b
    ) const
    {
      if (a.size() != b.size())
      {
        return a.size() < b.size();
      }

      for (std::size_t ii = 0; ii < a.size(); ++ii)
      {
        const int aNum = a[ii]->getNum();
        const int bNum = b[ii]->getNum();

        if (aNum != bNum)
        {
          return aNum < bNum;
        }
      }

      return false;
    }
  };

  std::map<std::vector<::MVertex *>, ::MTetrahedron *, MVertexVectorPtrLessThan>
    tet_face;

  dt__forAllRefAuto(tets, aTet)
  {
    dt__forFromToIndex(0, aTet->getNumFaces(), faceIndex)
    {
      faceCounter = faceCounter + 1;
      std::vector<::MVertex *> verts;
      aTet->getFace(faceIndex).getOrderedVertices(verts);
      dt__throwIf(verts.size() != 3, extractCoupleFaces);
      // canonical face vertex ordering by MVertex::getNum()
      std::sort(verts.begin(), verts.end(), MVertexPtrLessThan());
      auto it = tet_face.find(verts);
      if (it == tet_face.end())
      {
        tet_face[verts] = aTet;
      }
      else
      {
        tet_face.erase(it);
      }
    }
  }
  std::vector<::MTriangle *> faces;
  dt__forAllRefAuto(tet_face, it)
  {
    faces.push_back(new ::MTriangle(it.first));
  }

  return faces;
}

void dtOptimizeMeshGRegion::sortTetrahedraDeterministically(
  std::vector<::MTetrahedron *> &tets,
  const std::map<::MVertex *, ::MVertex *> &org_clone
) const
{
  std::sort(
    tets.begin(),
    tets.end(),
    [&org_clone](::MTetrahedron *a, ::MTetrahedron *b) {
      std::array<int, 4> aNum;
      std::array<int, 4> bNum;

      for (int ii = 0; ii < 4; ++ii)
      {
        ::MVertex *aVert = a->getVertex(ii);
        ::MVertex *bVert = b->getVertex(ii);

        //
        // Netgen operates on cloned vertices. Map them back to the
        // original vertices so that the deterministic original
        // MVertex::getNum() values are used.
        //
        auto aIt = org_clone.find(aVert);
        auto bIt = org_clone.find(bVert);

        aNum[ii] =
          (aIt != org_clone.end()) ? aIt->second->getNum() : aVert->getNum();

        bNum[ii] =
          (bIt != org_clone.end()) ? bIt->second->getNum() : bVert->getNum();
      }

      //
      // The orientation/order inside the MTetrahedron is not changed.
      // Only the four vertex numbers are copied into a sorted key.
      //
      std::sort(aNum.begin(), aNum.end());
      std::sort(bNum.begin(), bNum.end());

      return aNum < bNum;
    }
  );
}

} // namespace dtOO
