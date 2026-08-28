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

#include "dtOVMMesh.h"

#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include "dtLinearAlgebra.h"
#include "dtOMMesh.h"
#include <OpenVolumeMesh/FileManager/FileManager.hh>
#include <algorithm>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <gmsh/GEntity.h>
#include <gmsh/MElement.h>
#include <gmsh/MHexahedron.h>
#include <gmsh/MPrism.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MVertex.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {

dtOVMMesh::dtOVMMesh(int const &nV, dtInt const &nE) : ovmMesh()
{
  ::OpenVolumeMesh::VertexPropertyT<dtInt> mvProp =
    request_vertex_property<dtInt>("MVertex");
  set_persistent(mvProp);
  ::OpenVolumeMesh::CellPropertyT<dtInt> meProp =
    request_cell_property<dtInt>("MElement");
  set_persistent(meProp);

  if (nV)
    _mv.reserve(nV);
  if (nE)
    _me.reserve(nE);
}

dtOVMMesh::~dtOVMMesh() {}

ovmVertexH dtOVMMesh::addVertex(::MVertex *mv)
{
  ovmVertexH vH = ovmMesh::add_vertex(ovmPoint(mv->x(), mv->y(), mv->z()));
  dt__throwIf(!vH.is_valid(), addVertex());
  _mv.push_back(mv);
  request_vertex_property<dtInt>("MVertex")[vH] = _mv.size() - 1;
  _ovm_gmsh[mv] = vH;
  return vH;
}

ovmCellH dtOVMMesh::addCell(::MElement *me)
{
  // do not add an element twice.
  auto found = _ovm_gmshElement.find(me);
  if (found != _ovm_gmshElement.end())
    return found->second;

  // get handles of faces
  std::vector<ovmHalffaceH> handle(me->getNumFaces());
  dt__forFromToIndex(0, me->getNumFaces(), ii)
  {
    std::vector<::MVertex *> vv;
    me->getFace(ii).getOrderedVertices(vv);
    handle[ii] = addFace(vv);
  }

  // add cell
  ovmCellH cH = ovmMesh::add_cell(handle);
  // store additional mappings for
  _me.push_back(me);
  request_cell_property<dtInt>("MElement")[cH] = _me.size() - 1;
  _ovm_gmshElement[me] = cH;

  dt__throwIf(!cH.is_valid(), addCell());

  return cH;
}

ovmHalffaceH dtOVMMesh::addFace(::MVertex *mv0, ::MVertex *mv1, ::MVertex *mv2)
{
  std::vector<::MVertex *> vv(3);
  vv[0] = mv0;
  vv[1] = mv1;
  vv[2] = mv2;
  return addFace(vv);
}

ovmHalffaceH dtOVMMesh::addFace(std::vector<::MVertex *> const &mvs)
{
  std::vector<ovmVertexH> handle(mvs.size());
  dtInt already = 0;
  dt__forAllIndex(mvs, ii)
  {
    auto found = _ovm_gmsh.find(mvs[ii]);
    if (found != _ovm_gmsh.end())
    {
      handle[ii] = found->second;
      already = already + 1;
    }
    else
    {
      handle[ii] = addVertex(mvs[ii]);
    }
  }

  ovmHalffaceH hfH;
  if (already <= 1)
  {
    hfH = halfface_handle(ovmMesh::add_face(handle), 0);
  }
  else
  {
    hfH = find_halfface(handle);
    if (!hfH.is_valid())
    {
      hfH = halfface_handle(ovmMesh::add_face(handle), 0);
    }
    else if (!is_boundary(hfH))
    {
      hfH = opposite_halfface_handle(hfH);
    }
  }
  dt__throwIfWithMessage(
    !hfH.is_valid(), addFace(), << dt__eval(handle.size())
  );
  return hfH;
}

ovmVertexH const &dtOVMMesh::at(::MVertex const *const mv) const
{
  return _ovm_gmsh.at(mv);
}

::MVertex *dtOVMMesh::operator[](ovmVertexH const &vH)
{
  return _mv[request_vertex_property<dtInt>("MVertex")[vH]];
}

::MVertex const *const dtOVMMesh::at(ovmVertexH const &vH) const
{
  return _mv.at(
    const_cast<dtOVMMesh *>(this)->request_vertex_property<dtInt>("MVertex")[vH]
  );
}

::MElement *dtOVMMesh::operator[](ovmCellH const &cH)
{
  return _me[request_cell_property<dtInt>("MElement")[cH]];
}

::MElement const *const dtOVMMesh::at(ovmCellH const &cH) const
{
  return _me.at(
    const_cast<dtOVMMesh *>(this)->request_cell_property<dtInt>("MElement")[cH]
  );
}

void dtOVMMesh::replacePosition(ovmVertexH const &vH, dtPoint3 const &pp)
{
  ::MVertex *mv = (*this)[vH];
  mv->setXYZ(pp.x(), pp.y(), pp.z());
  ovmMesh::set_vertex(vH, ovmPoint(pp.x(), pp.y(), pp.z()));
}

std::vector<dtPoint3> dtOVMMesh::adjacentVertices(ovmVertexH const &vH) const
{
  std::vector<dtPoint3> pp;
  for (ovmVertexOHalfedgeI heIt = voh_iter(vH); heIt.valid(); ++heIt)
  {
    dt__throwIf(halfedge(*heIt).to_vertex() == vH, adjacentVertices());
    pp.push_back(dtGmshModel::extractPosition(at(halfedge(*heIt).to_vertex())));
  }
  return pp;
}

ovmCellH dtOVMMesh::addTet(
  ovmVertexH const &v0,
  ovmVertexH const &v1,
  ovmVertexH const &v2,
  ovmVertexH const &v3,
  bool const &correctOrientation
)
{
  ::MTetrahedron *tet =
    new ::MTetrahedron((*this)[v0], (*this)[v1], (*this)[v2], (*this)[v3]);

  // try to correct orientation; throw error if correctOrientation is set, but
  // correction is not possible
  if (correctOrientation && (tet->getVolumeSign() < 0))
  {
    tet->reverse();
    dt__throwIfWithMessage(
      tet->getVolumeSign() < 0, addTet(), << "Try to add an inversed element."
    );
  }
  return addCell(tet);
}

bool dtOVMMesh::removeTet(ovmCellH const &cH)
{
  // handle is invalid
  if (!cH.is_valid())
    return false;

  // cell is not a tetrahedron
  if (!isTetrahedron(cH))
    return false;

  // cell is deleted from internal mappings, vector _me, and finally in OVM;
  // _me is filled with a nullptr entry
  ::MElement *me = (*this)[cH];
  _ovm_gmshElement.erase(me);
  auto it = std::find(_me.begin(), _me.end(), me);
  if (it != _me.end())
  {
    delete *it;
    *it = nullptr;
  }
  else
    dt__throw(
      removeTet(),
      << "Remove cell that has a valid cell handle, but is not stored in _me "
         "vector."
    );
  // delete cell in OVM
  ovmMesh::delete_cell(cH);

  return true;
}

ovmVertexH dtOVMMesh::splitEdge(ovmVertexH const &v0, ovmVertexH const &v1)
{
  if (!v0.is_valid() || !v1.is_valid() || v0 == v1)
  {
    dt__warning(
      splitEdge(),
      << "Try to split an edge that has invalid vertex handles or the two "
         "handles are the same."
    );
    return ovmVertexH();
  }

  ovmEdgeH const &eH = ovmMesh::edge_handle(ovmMesh::find_halfedge(v0, v1));
  dt__throwIf(!eH.is_valid(), splitEdge);

  // Find all tetrahedra containing the edge.
  std::vector<ovmCellH> cells;
  for (ovmEdgeCellI ec_it = ec_iter(eH); ec_it.valid(); ++ec_it)
  {
    ovmCellH const cH = *ec_it;
    if (!isTetrahedron(cH))
      continue;
    cells.push_back(cH);
  }

  if (cells.empty())
  {
    dt__warning(splitEdge(), << "Edge is not connected to any cell.");
    return ovmVertexH();
  }

  // create midpoint
  dtPoint3 const p0 = dtGmshModel::extractPosition((*this)[v0]);
  dtPoint3 const p1 = dtGmshModel::extractPosition((*this)[v1]);
  ::MVertex *mv = new ::MVertex(
    0.5 * (p0.x() + p1.x()), 0.5 * (p0.y() + p1.y()), 0.5 * (p0.z() + p1.z())
  );
  // the new point is set to the GEntity with the highest dimension;
  // additionally a warning is printed, if the point lies not in a volume
  if (this->at(v1)->onWhat()->dim() > this->at(v0)->onWhat()->dim())
  {
    mv->setEntity(this->at(v1)->onWhat());
  }
  else
  {
    mv->setEntity(this->at(v0)->onWhat());
  }
  dt__warnIf(mv->onWhat()->dim() < 3, splitEdge());

  // add vertex to OVM
  ovmVertexH const vm = addVertex(mv);

  // store vertices of old tetrahedral topology
  std::vector<std::vector<ovmVertexH>> oldCells;
  dt__forAllRefAuto(cells, cH)
  {
    std::vector<ovmVertexH> vv;
    for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
    {
      vv.push_back(*v_it);
    }
    oldCells.push_back(vv);
  }

  // remove old tetrahedra
  dt__forAllRefAuto(cells, cH) dt__throwIf(removeTet(cH) == false, splitEdge());

  // replace every tetrahedron by two
  dt__forAllRefAuto(oldCells, vv)
  {
    std::vector<ovmVertexH> other;
    dt__forAllRefAuto(vv, vH)
    {
      if (vH != v0 && vH != v1)
        other.push_back(vH);
    }

    if (other.size() != 2)
      continue;
    addTet(v0, vm, other[0], other[1], true);
    addTet(vm, v1, other[0], other[1], true);
  }

  return vm;
}

bool dtOVMMesh::trySplitEdge(
  ovmVertexH const &v0,
  ovmVertexH const &v1,
  std::function<bool(ovmVertexH const &)> const &accept
)
{
  if (!v0.is_valid() || !v1.is_valid() || v0 == v1)
    return false;

  // find edge
  ovmEdgeH const &eH = ovmMesh::edge_handle(ovmMesh::find_halfedge(v0, v1));
  dt__throwIf(!eH.is_valid(), splitEdge);

  // store original tetrahedral topology
  std::vector<std::vector<ovmVertexH>> oldTets;
  for (ovmEdgeCellI ec_it = ec_iter(eH); ec_it.valid(); ++ec_it)
  {
    ovmCellH const cH = *ec_it;
    if (!isTetrahedron(cH))
      return false;
    std::vector<ovmVertexH> vertices;
    for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
      vertices.push_back(*v_it);
    oldTets.push_back(vertices);
  }

  if (oldTets.empty())
  {
    dt__warning(trySplitEdge(), << "Edge is not connected to any cell.");
    return false;
  }

  ovmVertexH const vNew = splitEdge(v0, v1);

  // split was not possible; so no new vertices and cells were inserted
  if (!vNew.is_valid())
    return false;

  // check if this split is accepted by the user defined functor; if yes the
  // new vertex including new elements were inserted
  if (accept(vNew))
    return true;

  //
  // split is not acceptable; revert to old state of the mesh
  //
  // find all new cells connected to the new vertex
  std::vector<ovmCellH> newCells;
  for (ovmVertexCellI c_it = vc_iter(vNew); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;
    if (isTetrahedron(cH))
      newCells.push_back(cH);
  }
  // remove cells
  dt__forAllRefAuto(newCells, cH)
    dt__throwIf(removeTet(cH) == false, trySplitEdge());

  // remove new vertex
  removeVertex(vNew);

  // restore original tetrahedra
  dt__forAllRefAuto(oldTets, vertices)
    addTet(vertices[0], vertices[1], vertices[2], vertices[3]);

  return false;
}

void dtOVMMesh::removeVertex(ovmVertexH const &vH)
{
  int nConnections = 0;
  for (ovmVertexCellI vc_it = ovmMesh::vc_iter(vH); vc_it.valid(); ++vc_it)
  {
    if (vc_it.valid())
      nConnections = nConnections + 1;
  }
  dt__throwIfWithMessage(
    nConnections > 0, removeVertex(), << "nConnections = " << nConnections
  );

  _ovm_gmsh.erase((*this)[vH]);
  ::MVertex const *mv = this->operator[](vH);

  auto it = std::find(_mv.begin(), _mv.end(), mv);
  if (it != _mv.end())
  {
    delete *it;
    *it = nullptr;
  }
  else
    dt__throw(
      removeVertex(),
      << "Remove vertex that has a valid vertex handle, but is not stored in "
         "_mv vector."
    );
  ovmMesh::delete_vertex(vH);
}

bool dtOVMMesh::isTetrahedron(ovmCellH const &cH) const
{
  if (dynamic_cast<::MTetrahedron const *>(this->at(cH)))
  {
    return true;
  }
  return false;
}

bool dtOVMMesh::isPyramid(ovmCellH const &cH) const
{
  if (dynamic_cast<::MPyramid const *>(this->at(cH)))
  {
    return true;
  }
  return false;
}

bool dtOVMMesh::isPrism(ovmCellH const &cH) const
{
  if (dynamic_cast<::MPrism const *>(this->at(cH)))
  {
    return true;
  }
  return false;
}

bool dtOVMMesh::isHexahedron(ovmCellH const &cH) const
{
  if (dynamic_cast<::MHexahedron const *>(this->at(cH)))
  {
    return true;
  }
  return false;
}

bool dtOVMMesh::tryRemoveTet(
  ovmCellH const &cH,
  ovmCellH const &neighbour,
  std::function<bool(std::vector<ovmVertexH> const &)> const &accept
)
{
  if (!cH.is_valid() || !neighbour.is_valid())
    return false;

  if (!isTetrahedron(cH) || !isTetrahedron(neighbour))
    return false;

  // vertices of first tetrahedron
  std::vector<ovmVertexH> tet0;
  for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
  {
    tet0.push_back(*v_it);
  }

  // vertices of neighbouring tetrahedron
  std::vector<ovmVertexH> tet1;
  for (ovmCellVertexI v_it = cv_iter(neighbour); v_it.valid(); ++v_it)
  {
    tet1.push_back(*v_it);
  }

  // find the three shared vertices
  std::vector<ovmVertexH> shared;
  for (ovmVertexH const &vH : tet0)
  {
    for (ovmVertexH const &vH2 : tet1)
    {
      if (vH == vH2)
      {
        shared.push_back(vH);
        break;
      }
    }
  }

  if (shared.size() != 3)
    return false;

  // find opposite vertex of first tetrahedron
  ovmVertexH d;
  for (ovmVertexH const &vH : tet0)
  {
    bool isShared = false;
    for (ovmVertexH const &sH : shared)
    {
      if (vH == sH)
      {
        isShared = true;
        break;
      }
    }
    if (!isShared)
    {
      d = vH;
      break;
    }
  }

  // find opposite vertex of second tetrahedron
  ovmVertexH e;
  for (ovmVertexH const &vH : tet1)
  {
    bool isShared = false;
    for (ovmVertexH const &sH : shared)
    {
      if (vH == sH)
      {
        isShared = true;
        break;
      }
    }
    if (!isShared)
    {
      e = vH;
      break;
    }
  }

  if (!d.is_valid() || !e.is_valid())
    return false;

  // Store original topology.
  std::vector<ovmVertexH> oldTet0 = tet0;
  std::vector<ovmVertexH> oldTet1 = tet1;

  // Remove original tetrahedra
  if (!removeTet(cH))
    return false;
  if (!removeTet(neighbour))
  {
    addTet(oldTet0[0], oldTet0[1], oldTet0[2], oldTet0[3]);
    return false;
  }

  // Create 2 -> 3 configuration.
  ovmCellH const c0 = addTet(shared[0], shared[1], d, e, true);
  ovmCellH const c1 = addTet(shared[1], shared[2], d, e, true);
  ovmCellH const c2 = addTet(shared[2], shared[0], d, e, true);

  dt__throwIf(!c0.is_valid(), tryRemoveTet());
  dt__throwIf(!c1.is_valid(), tryRemoveTet());
  dt__throwIf(!c2.is_valid(), tryRemoveTet());

  // five vertices affected by the 2 -> 3 flip
  std::vector<ovmVertexH> affectedVertices(5);
  affectedVertices[0] = shared[0];
  affectedVertices[1] = shared[1];
  affectedVertices[2] = shared[2];
  affectedVertices[3] = d;
  affectedVertices[4] = e;

  // check new configuration with functor
  if (accept(affectedVertices))
    return true;

  // Rollback
  removeTet(c0);
  removeTet(c1);
  removeTet(c2);
  addTet(oldTet0[0], oldTet0[1], oldTet0[2], oldTet0[3]);
  addTet(oldTet1[0], oldTet1[1], oldTet1[2], oldTet1[3]);

  return false;
}

bool dtOVMMesh::applyTo(dtGmshRegion *dtgr)
{
  if (!dtgr)
    return false;

  dtgr->mesh_vertices.clear();
  dtgr->tetrahedra.clear();
  dtgr->hexahedra.clear();
  dtgr->prisms.clear();
  dtgr->pyramids.clear();
  dtgr->polyhedra.clear();

  if (ovmMesh::needs_garbage_collection())
  {
    dt__info(applyTo(), << "Collect garbage.");
    ovmMesh::collect_garbage();
  }

  for (ovmVertexI v_it = v_iter(); v_it.valid(); ++v_it)
  {
    ovmVertexH const vH = *v_it;
    ::MVertex *mv = (*this)[vH];
    if (!mv)
      continue;
    if (mv->onWhat() != dtgr)
      continue;

    ovmPoint const &p = vertex(vH);
    dt__throwIf(
      !analyticGeometry::inXYZTolerance(dtLinearAlgebra::length(
        dtVector3(p[0] - mv->x(), p[1] - mv->y(), p[2] - mv->z())
      )),
      applyTo()
    );
    dtgr->addMeshVertex(mv);
  }

  for (ovmCellI c_it = c_iter(); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;

    ::MElement *me = (*this)[cH];

    if (!me)
      continue;

    dtgr->addElement(me);
  }

  return true;
}

} // namespace dtOO
