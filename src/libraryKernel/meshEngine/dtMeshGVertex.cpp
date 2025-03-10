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

#include "dtMeshGVertex.h"

#include "dtGmshVertex.h"
// #include <gmsh/meshGEdge.h>
#include "dtMeshOperatorFactory.h"
#include <gmsh/MPoint.h>
#include <gmsh/MVertex.h>

namespace dtOO {
bool dtMeshGVertex::_registrated = dtMeshOperatorFactory::registrate(
  dt__tmpPtr(dtMeshGVertex, new dtMeshGVertex())
);

dtMeshGVertex::dtMeshGVertex() : dtMesh0DOperator() {}

dtMeshGVertex::dtMeshGVertex(const dtMeshGVertex &orig) : dtMesh0DOperator(orig)
{
}

dtMeshGVertex::~dtMeshGVertex() {}

void dtMeshGVertex::operator()(dtGmshVertex *dtgv)
{

  Msg::Info("Meshing vertex %d ( dtGmshVertex )", dtgv->tag());

  //
  // mesh vertex
  //
  if (dtgv->mesh_vertices.empty())
  {
    dtgv->mesh_vertices.push_back(
      new ::MVertex(dtgv->x(), dtgv->y(), dtgv->z(), dtgv)
    );
  }
  if (dtgv->points.empty())
  {
    dtgv->points.push_back(new ::MPoint(dtgv->mesh_vertices.back()));
  }

  dtgv->_status = ::GEntity::MeshGenerationStatus::DONE;
}
} // namespace dtOO
