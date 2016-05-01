#include "dtMeshGFace.h"

#include "dtGmshFace.h"
#include <gmsh/meshGFace.h>

namespace dtOO {
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

