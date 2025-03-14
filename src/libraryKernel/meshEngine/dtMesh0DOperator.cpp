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

#include "dtMesh0DOperator.h"

#include "dtGmshVertex.h"
#include "jsonHeaven/jsonPrimitive.h"
#include <gmsh/MPoint.h>
#include <gmsh/MVertex.h>
#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
dtMesh0DOperator::dtMesh0DOperator() {}

dtMesh0DOperator::~dtMesh0DOperator() {}

void dtMesh0DOperator::jInit(
  jsonPrimitive const &jE,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMeshOperator::jInit(jE, bC, cV, aF, aG, bV, mO);
}

void dtMesh0DOperator::init(
  ::QDomElement const &element,
  baseContainer const *const bC,
  lvH_constValue const *const cV,
  lvH_analyticFunction const *const aF,
  lvH_analyticGeometry const *const aG,
  lvH_boundedVolume const *const bV,
  lvH_dtMeshOperator const *const mO
)
{
  dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
}

void dtMesh0DOperator::correspondingVertex(
  dtGmshVertex *master, dtGmshVertex *slave
)
{
  Msg::Info(
    "Corresponding vertex %d (master) -> %d (slave)",
    master->tag(),
    slave->tag()
  );
  //
  // corresponding vertex
  //
  if (slave->mesh_vertices.empty())
  {
    slave->mesh_vertices.push_back(
      new ::MVertex(slave->x(), slave->y(), slave->z(), slave)
    );
  }
  if (slave->points.empty())
  {
    slave->points.push_back(new ::MPoint(slave->mesh_vertices.back()));
  }

  slave->correspondingVertices[slave->mesh_vertices[0]] =
    master->mesh_vertices[0];
}
} // namespace dtOO
