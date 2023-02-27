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

