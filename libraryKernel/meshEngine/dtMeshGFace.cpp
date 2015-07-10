#include "dtMeshGFace.h"

#include "dtGmshFace.h"
#include <gmsh/meshGFace.h>

namespace dtOO {
  dtMeshGFace::dtMeshGFace() {
  }

  dtMeshGFace::dtMeshGFace(const dtMeshGFace& orig) {
  }

  dtMeshGFace::~dtMeshGFace() {
  }

  void dtMeshGFace::operator()( dtGmshFace * dtgf) {
      ::meshGFace()( dtgf );
  }
}

