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

