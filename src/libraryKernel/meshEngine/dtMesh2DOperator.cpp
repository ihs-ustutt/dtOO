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

#include "dtMesh2DOperator.h"

#include "dtGmshFace.h"
#include <gmsh/meshGFace.h>
#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
dtMesh2DOperator::dtMesh2DOperator() {}

dtMesh2DOperator::dtMesh2DOperator(const dtMesh2DOperator &orig) {}

dtMesh2DOperator::~dtMesh2DOperator() {}

void dtMesh2DOperator::jInit(
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

void dtMesh2DOperator::init(
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

void dtMesh2DOperator::copyMesh(dtGmshFace *from, dtGmshFace *to)
{
  Msg::Info(
    "Copy mesh from surface %d to surface %d ( dtMesh2DOperator )",
    from->tag(),
    to->tag()
  );
  meshGFace()(to);
}
} // namespace dtOO
