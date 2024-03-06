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

#include "dtMeshAndOrientGEdge.h"

#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>
#include "dtMeshOperatorFactory.h"

namespace dtOO {
  bool dtMeshAndOrientGEdge::_registrated 
  =
  dtMeshOperatorFactory::registrate(
    dt__tmpPtr(dtMeshAndOrientGEdge, new dtMeshAndOrientGEdge())
  );
 
 
  dtMeshAndOrientGEdge::dtMeshAndOrientGEdge() : dtMeshGEdge() {
  }

  dtMeshAndOrientGEdge::dtMeshAndOrientGEdge(
    const dtMeshAndOrientGEdge& orig
  ) : dtMeshGEdge(orig) {
    
  }

  dtMeshAndOrientGEdge::~dtMeshAndOrientGEdge() {
  } 

  void dtMeshAndOrientGEdge::operator()( dtGmshEdge * dtge) {
      dtMeshGEdge()( dtge );
      ::orientMeshGEdge()(dtge);
  }
}
