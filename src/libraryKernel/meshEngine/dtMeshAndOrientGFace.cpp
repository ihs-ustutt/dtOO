#include "dtMeshAndOrientGFace.h"

#include "dtGmshFace.h"
#include <gmsh/meshGFace.h>

namespace dtOO {
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

