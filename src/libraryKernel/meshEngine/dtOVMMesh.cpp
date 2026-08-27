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

#include "analyticGeometryHeaven/analyticGeometry.h"
#include "dtGmshModel.h"
#include "dtGmshRegion.h"
#include "dtLinearAlgebra.h"
#include "dtOMMesh.h"

#include <OpenVolumeMesh/FileManager/FileManager.hh>

#include <gmsh/GEntity.h>
#include <gmsh/MElement.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MVertex.h>

#include <algorithm>

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
  // Do not add an element twice.
  //
  auto found = _ovm_gmshElement.find(me);
  if (found != _ovm_gmshElement.end())
    return found->second;

  std::vector<ovmHalffaceH> handle(me->getNumFaces());
  for (int ii = 0; ii < me->getNumFaces(); ++ii)
  {
    std::vector<::MVertex *> vv;
    me->getFace(ii).getOrderedVertices(vv);
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
  std::vector<ovmVertexH> handle(mv.size());
  dtInt already = 0;
  for (std::size_t ii = 0; ii < mv.size(); ++ii)
  {
    auto found = _ovm_gmsh.find(mv[ii]);
    if (found != _ovm_gmsh.end())
    {
      handle[ii] = found->second;
      ++already;
    }
    else
    {
      handle[ii] = addVertex(mv[ii]);
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
  if (correctOrientation && (tet->getVolumeSign() < 0))
  {
    tet->reverse();
    dt__throwIfWithMessage(
      tet->getVolumeSign() < 0, addTet(), << "Try to add an inversed element."
    );
  }
  dt__warnIfWithMessage(
    tet->getVolume() < 0,
    addTet(),
    << "correctOrientation = " << correctOrientation
  );
  return addCell(tet);
}

bool dtOVMMesh::removeTet(ovmCellH const &cH)
{
  if (!cH.is_valid())
    return false;
  ::MElement *me = (*this)[cH];
  if (!dynamic_cast<::MTetrahedron *>(me))
    return false;

  _ovm_gmshElement.erase(me);
  auto it = std::find(_me.begin(), _me.end(), me);
  if (it != _me.end())
  {
    delete *it;
    *it = nullptr;
  }
  ovmMesh::delete_cell(cH);
  return true;
}

ovmVertexH dtOVMMesh::splitEdge(ovmVertexH const &v0, ovmVertexH const &v1)
{
  if (!v0.is_valid() || !v1.is_valid() || v0 == v1)
    return ovmVertexH();

  // Find all tetrahedra containing the edge.
  std::vector<ovmCellH> cells;
  for (ovmVertexCellI c_it = vc_iter(v0); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;
    ::MElement *me = (*this)[cH];
    if (!dynamic_cast<::MTetrahedron *>(me))
      continue;
    bool containsV1 = false;
    for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
    {
      if (*v_it == v1)
      {
        containsV1 = true;
        break;
      }
    }
    if (containsV1)
      cells.push_back(cH);
  }
  if (cells.empty())
    return ovmVertexH();

  //
  // Create midpoint.
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
  // Store old tetrahedral topology.
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
    dt__throwIf(removeTet(cH) == false, splitEdge());

  //
  // Replace every tetrahedron by two.
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

  // Store original tetrahedral topology.
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

  ovmVertexH const vNew = splitEdge(v0, v1);

  if (!vNew.is_valid())
    return false;

  if (accept(vNew))
    return true;

  //
  // Rollback.
  //
  std::vector<ovmCellH> newCells;
  for (ovmVertexCellI c_it = vc_iter(vNew); c_it.valid(); ++c_it)
  {
    ovmCellH const cH = *c_it;
    if (dynamic_cast<::MTetrahedron *>((*this)[cH]))
      newCells.push_back(cH);
  }

  for (ovmCellH const &cH : newCells)
  {
    dt__throwIf(removeTet(cH) == false, trySplitEdge());
  }

  //
  // Delete midpoint vertex.
  //
  _ovm_gmsh.erase((*this)[vNew]);
  delete (*this)[vNew];
  _mv.pop_back();
  ovmMesh::delete_vertex(vNew);

  //
  // Restore original tetrahedra.
  //
  for (std::vector<ovmVertexH> const &vertices : oldTets)
  {
    addTet(vertices[0], vertices[1], vertices[2], vertices[3]);
  }

  return false;
}

bool dtOVMMesh::tryRemoveTet(
  ovmCellH const &cH,
  std::function<bool(std::vector<ovmVertexH> const &)> const &accept
)
{
  if (!cH.is_valid())
    return false;

  ::MElement *me0 = (*this)[cH];

  if (!dynamic_cast<::MTetrahedron *>(me0))
    return false;

  //
  // Vertices of first tetrahedron.
  //
  std::vector<ovmVertexH> tet0;

  for (ovmCellVertexI v_it = cv_iter(cH); v_it.valid(); ++v_it)
  {
    tet0.push_back(*v_it);
  }

  if (tet0.size() != 4)
    return false;

  //
  // Find neighbouring tetrahedron.
  //
  ovmCellH neighbour;
  std::vector<ovmVertexH> shared;

  for (ovmCellI c_it = c_iter(); c_it.valid(); ++c_it)
  {
    ovmCellH const cH2 = *c_it;

    if (cH2 == cH)
      continue;

    ::MElement *me2 = (*this)[cH2];

    if (!dynamic_cast<::MTetrahedron *>(me2))
      continue;

    std::vector<ovmVertexH> common;

    for (ovmVertexH const &vH : tet0)
    {
      for (ovmCellVertexI v_it = cv_iter(cH2); v_it.valid(); ++v_it)
      {
        if (vH == *v_it)
        {
          common.push_back(vH);
          break;
        }
      }
    }

    if (common.size() == 3)
    {
      neighbour = cH2;
      shared = common;
      break;
    }
  }

  if (!neighbour.is_valid())
    return false;

  //
  // Vertices of neighbouring tetrahedron.
  //
  std::vector<ovmVertexH> tet1;

  for (ovmCellVertexI v_it = cv_iter(neighbour); v_it.valid(); ++v_it)
  {
    tet1.push_back(*v_it);
  }

  if (tet1.size() != 4)
    return false;

  //
  // Find opposite vertices.
  //
  ovmVertexH d;
  ovmVertexH e;

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

  ovmVertexH const a = shared[0];
  ovmVertexH const b = shared[1];
  ovmVertexH const c = shared[2];

  //
  // Store original topology.
  //
  std::vector<ovmVertexH> oldTet0 = tet0;
  std::vector<ovmVertexH> oldTet1 = tet1;

  //
  // Remove original configuration.
  //
  if (!removeTet(cH))
    return false;

  if (!removeTet(neighbour))
  {
    addTet(oldTet0[0], oldTet0[1], oldTet0[2], oldTet0[3]);

    return false;
  }

  //
  // Create 2->3 configuration.
  //
  ovmCellH const c0 = addTet(a, b, d, e, true);

  ovmCellH const c1 = addTet(b, c, d, e, true);

  ovmCellH const c2 = addTet(c, a, d, e, true);

  if (!c0.is_valid() || !c1.is_valid() || !c2.is_valid())
  {
    if (c0.is_valid())
      removeTet(c0);

    if (c1.is_valid())
      removeTet(c1);

    if (c2.is_valid())
      removeTet(c2);

    addTet(oldTet0[0], oldTet0[1], oldTet0[2], oldTet0[3]);

    addTet(oldTet1[0], oldTet1[1], oldTet1[2], oldTet1[3]);

    return false;
  }

  std::vector<ovmVertexH> affectedVertices;

  affectedVertices.push_back(a);
  affectedVertices.push_back(b);
  affectedVertices.push_back(c);
  affectedVertices.push_back(d);
  affectedVertices.push_back(e);

  if (accept(affectedVertices))
    return true;

  //
  // Rollback.
  //
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
