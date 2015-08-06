#include "dtMeshAndOrientGEdge.h"

#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>

namespace dtOO {
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

