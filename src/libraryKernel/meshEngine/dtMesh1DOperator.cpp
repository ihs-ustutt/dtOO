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

#include "dtMesh1DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>

namespace dtOO {
  dtMesh1DOperator::dtMesh1DOperator() {
  }

  dtMesh1DOperator::~dtMesh1DOperator() {
  }

  void dtMesh1DOperator::jInit(
    jsonPrimitive const & jE,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtMeshOperator const * const mO    
  ) {
    dtMeshOperator::jInit(jE, bC, cV, aF, aG, bV, mO);
  }

  void dtMesh1DOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtMeshOperator const * const mO    
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }
  
  void dtMesh1DOperator::copyMesh( dtGmshEdge * from, dtGmshEdge * to) {
     Msg::Info(
      "Copy mesh from edge %d to edge %d ( dtMesh1DOperator )", 
      from->tag(), to->tag()
    );
    meshGEdge()( to );
  }
}
