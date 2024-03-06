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

#include "dtMeshGFace.h"

#include "dtGmshFace.h"
#include <gmsh/meshGFace.h>
#include "dtMeshOperatorFactory.h"

namespace dtOO {
  bool dtMeshGFace::_registrated 
  =
  dtMeshOperatorFactory::registrate(
    dt__tmpPtr(dtMeshGFace, new dtMeshGFace())
  );
 
  dtMeshGFace::dtMeshGFace() : dtMesh2DOperator() {
  }

  dtMeshGFace::dtMeshGFace(const dtMeshGFace& orig) : dtMesh2DOperator(orig) {
  }

  dtMeshGFace::~dtMeshGFace() {
  }

  void dtMeshGFace::operator()( dtGmshFace * dtgf) {
    ::meshGFace()( dtgf );
  }
}
