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

#include "dtMeshAndOrientGFace.h"

#include "dtGmshFace.h"
#include <gmsh/meshGFace.h>
#include "dtMeshOperatorFactory.h"

namespace dtOO {
  bool dtMeshAndOrientGFace::_registrated 
  =
  dtMeshOperatorFactory::registrate(
    dt__tmpPtr(dtMeshAndOrientGFace, new dtMeshAndOrientGFace())
  );
 
  dtMeshAndOrientGFace::dtMeshAndOrientGFace() : dtMeshGFace() {
  }

  dtMeshAndOrientGFace::dtMeshAndOrientGFace(
    const dtMeshAndOrientGFace& orig
  ) : dtMeshGFace(orig) {
  }

  dtMeshAndOrientGFace::~dtMeshAndOrientGFace() {
  }

  void dtMeshAndOrientGFace::operator()( dtGmshFace * dtgf) {
    dtMeshGFace()( dtgf );
    ::orientMeshGFace()(dtgf);
  }
}
