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

#include "dtMeshGEdge.h"

#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>
#include <gmsh/Context.h>
#include "dtMeshOperatorFactory.h"

namespace dtOO {
  bool dtMeshGEdge::_registrated 
  =
  dtMeshOperatorFactory::registrate(
    dt__tmpPtr(dtMeshGEdge, new dtMeshGEdge())
  );
 
  dtMeshGEdge::dtMeshGEdge() : dtMesh1DOperator() {
  }

  dtMeshGEdge::dtMeshGEdge(const dtMeshGEdge& orig) : dtMesh1DOperator(orig) {
  }

  dtMeshGEdge::~dtMeshGEdge() {
  } 

  void dtMeshGEdge::operator()( dtGmshEdge * dtge) {
    //
    // reset temporarily lc parameter of gmsh
    // --> prevents really slow meshing
    //
    double lcOld = CTX::instance()->lc;
    CTX::instance()->lc = 1.e+12 * CTX::instance()->mesh.lcIntegrationPrecision;
    ::meshGEdge()( dtge );
    CTX::instance()->lc = lcOld;
  }
}
