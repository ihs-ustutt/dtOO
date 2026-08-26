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
#include "dtOMMesh.h"
#include <OpenVolumeMesh/FileManager/FileManager.hh>
#include <gmsh/GEntity.h>
#include <gmsh/MElement.h>
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
  //
  // check if already in mesh
  //
  if (_ovm_gmshElement.find(me) != _ovm_gmshElement.end())
  {
    return _ovm_gmshElement[me];
  }

  ::MElement *nCMElement = const_cast<::MElement *>(me);
  std::vector<ovmHalffaceH> handle(nCMElement->getNumFaces());

  dt__forFromToIndex(0, nCMElement->getNumFaces(), ii)
  {
    std::vector<::MVertex *> vv;
    nCMElement->getFace(ii).getOrderedVertices(vv);
    handle[ii] = addFace(vv);
  }
  ovmCellH cH = ovmMesh::add_cell(handle);

  _me.push_back(me);
  request_cell_property<dtInt>("MElement")[cH] = _me.size() - 1;
  _ovm_gmshElement[me] = cH;
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

ovmHalffaceH dtOVMMesh::addFace(std::vector<::MVertex *> const &mv)
{
  std::vector<ovmVertexH> handle(mv.size(), ovmVertexH());
  dtInt already = 0;

  dt__forFromToIndex(0, mv.size(), ii)
  {

    if (_ovm_gmsh.find(mv[ii]) != _ovm_gmsh.end())
    {
      handle[ii] = _ovm_gmsh[mv[ii]];
      already++;
    }
    else
      handle[ii] = addVertex(mv[ii]);
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
    else
    {
      if (!is_boundary(hfH))
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
  //
  // update position
  //
  ::MVertex *mv = this->operator[](vH);
  mv->setXYZ(pp.x(), pp.y(), pp.z());
  ovmMesh::set_vertex(vH, ovmPoint(pp.x(), pp.y(), pp.z()));
}

void dtOVMMesh::makePartition(dtInt const &num) const
{
  dt__forAllRefAuto(_me, anEl)
  {
    const_cast<::MElement *>(anEl)->setPartition(num);
  }
}
std::vector<dtPoint3> dtOVMMesh::adjacentVertices(ovmVertexH const &vH) const
{
  std::vector<dtPoint3> pp;
  for (ovmVertexOHalfedgeI heIt = voh_iter(vH); heIt.valid(); heIt++)
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
  ovmVertexH const &v3
)
{
  std::vector<::MVertex *> vv(4);

  vv[0] = (*this)[v0];
  vv[1] = (*this)[v1];
  vv[2] = (*this)[v2];
  vv[3] = (*this)[v3];

  ::MTetrahedron *tet = new ::MTetrahedron(vv[0], vv[1], vv[2], vv[3]);

  return addCell(tet);
}

bool dtOVMMesh::removeTet(ovmCellH const &cH)
{
  if (!cH.is_valid())
    return false;

  ::MElement *me = this->operator[](cH);

  if (!dynamic_cast<::MTetrahedron *>(me))
    return false;

  //
  // remove the Gmsh -> OVM mapping
  //
  _ovm_gmshElement.erase(me);

  auto it = std::find(_me.begin(), _me.end(), me);

  if (it != _me.end())
    //*it = nullptr;
    delete *it;
  //
  // delete the OVM cell
  //
  ovmMesh::delete_cell(cH);

  return true;
}

ovmVertexH dtOVMMesh::splitEdge(ovmVertexH const &v0, ovmVertexH const &v1)
{
  if (!v0.is_valid() || !v1.is_valid() || v0 == v1)
    return ovmVertexH();

  //
  // find all tetrahedra containing the edge.
  //
  std::vector<ovmCellH> cells;

  for (ovmVertexCellI c_it = vc_iter(v0); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;

    ::MElement *me = (*this)[cH];

    if (!dynamic_cast<::MTetrahedron *>(me))
      continue;

    bool hasV1 = false;

    for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
    {
      if (*v_it == v1)
      {
        hasV1 = true;
        break;
      }
    }

    if (hasV1)
      cells.push_back(cH);
  }

  if (cells.empty())
    return ovmVertexH();

  //
  // Create midpoint vertex.
  //
  dtPoint3 const p0 = dtGmshModel::extractPosition((*this)[v0]);

  dtPoint3 const p1 = dtGmshModel::extractPosition((*this)[v1]);

  ::MVertex *mv = new ::MVertex(
    0.5 * (p0.x() + p1.x()), 0.5 * (p0.y() + p1.y()), 0.5 * (p0.z() + p1.z())
  );
  if (this->at(v1)->onWhat()->dim() > this->at(v0)->onWhat()->dim())
  {
    mv->setEntity(this->at(v1)->onWhat());
  }
  else
  {
    mv->setEntity(this->at(v0)->onWhat());
  }

  ovmVertexH const vm = addVertex(mv);

  //
  // Store the old tetrahedra.
  //
  std::vector<std::vector<ovmVertexH>> oldCells;

  for (ovmCellH const &cH : cells)
  {
    std::vector<ovmVertexH> vv;

    for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
    {
      vv.push_back(*v_it);
    }

    oldCells.push_back(vv);
  }

  //
  // Remove old tetrahedra.
  //
  for (ovmCellH const &cH : cells)
    removeTet(cH);

  //
  // Split every tetrahedron into two tetrahedra.
  //
  for (std::vector<ovmVertexH> const &vv : oldCells)
  {
    std::vector<ovmVertexH> other;

    for (ovmVertexH const &vH : vv)
    {
      if (vH != v0 && vH != v1)
        other.push_back(vH);
    }

    if (other.size() != 2)
      continue;

    addTet(v0, vm, other[0], other[1]);

    addTet(vm, v1, other[0], other[1]);
  }

  return vm;
}

bool dtOVMMesh::applyTo(dtGmshRegion *dtgr)
{
  if (!dtgr)
    return false;

  //
  // clear dtgr
  //
  dtgr->mesh_vertices.clear();
  dtgr->tetrahedra.clear();
  dtgr->hexahedra.clear();
  dtgr->prisms.clear();
  dtgr->pyramids.clear();
  dtgr->polyhedra.clear();

  //
  // Update positions of all vertices.
  //
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

    mv->setXYZ(p[0], p[1], p[2]);
    dtgr->addMeshVertex(mv);
  }

  for (ovmCellI c_it = c_iter(); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;

    ::MElement *me = (*this)[cH];

    if (!me)
      continue;

    // ovmPoint const &p = vertex(vH);

    // mv->setXYZ(p[0], p[1], p[2]);
    dtgr->addElement(me);
  }

  return true;
}

bool dtOVMMesh::trySplitEdge(
  ovmVertexH const &v0,
  ovmVertexH const &v1,
  std::function<bool(ovmVertexH const &)> const &accept
)
{
  if (!v0.is_valid() || !v1.is_valid() || v0 == v1)
    return false;

  //
  // Store all tetrahedra which are affected by the split.
  //
  std::vector<std::vector<ovmVertexH>> oldTets;

  for (ovmVertexCellI c_it = vc_iter(v0); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;

    bool containsV1 = false;

    for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
    {
      if (*v_it == v1)
      {
        containsV1 = true;
        break;
      }
    }

    if (!containsV1)
      continue;

    ::MElement *me = (*this)[cH];

    if (!dynamic_cast<::MTetrahedron *>(me))
      return false;

    std::vector<ovmVertexH> vertices;

    for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
    {
      vertices.push_back(*v_it);
    }

    if (vertices.size() != 4)
      return false;

    oldTets.push_back(vertices);
  }

  if (oldTets.empty())
    return false;

  //
  // Perform the split.
  //
  ovmVertexH const vNew = splitEdge(v0, v1);

  if (!vNew.is_valid())
    return false;

  //
  // Let the caller evaluate the new mesh.
  //
  if (accept(vNew))
    return true;

  //
  // ----------------------------------------------------------
  // Rollback
  // ----------------------------------------------------------
  //

  //
  // Remove all tetrahedra incident to the new vertex.
  //
  std::vector<ovmCellH> newCells;

  for (ovmVertexCellI c_it = vc_iter(vNew); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;

    ::MElement *me = (*this)[cH];

    if (dynamic_cast<::MTetrahedron *>(me))
      newCells.push_back(cH);
  }

  for (ovmCellH const &cH : newCells)
  {
    ::MElement *me = (*this)[cH];

    _ovm_gmshElement.erase(me);

    ovmMesh::delete_cell(cH);
  }

  //
  // Remove the newly created vertex.
  //
  _ovm_gmsh.erase((*this)[vNew]);

  _mv.pop_back();

  ovmMesh::delete_vertex(vNew);

  //
  // Recreate the original tetrahedra.
  //
  for (std::vector<ovmVertexH> const &vertices : oldTets)
  {
    if (vertices.size() != 4)
      return false;

    //
    // The original Gmsh element is no longer needed by the
    // OVM topology. A new MElement is created here.
    //
    ::MVertex *mv0 = (*this)[vertices[0]];
    ::MVertex *mv1 = (*this)[vertices[1]];
    ::MVertex *mv2 = (*this)[vertices[2]];
    ::MVertex *mv3 = (*this)[vertices[3]];

    ::MTetrahedron *tet = new ::MTetrahedron(mv0, mv1, mv2, mv3);

    addCell(tet);
  }

  return false;
}
} // namespace dtOO
