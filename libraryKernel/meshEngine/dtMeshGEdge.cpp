#include "dtMeshGEdge.h"

#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>

namespace dtOO {
  dtMeshGEdge::dtMeshGEdge() {
  }

  dtMeshGEdge::dtMeshGEdge(const dtMeshGEdge& orig) {
  }

  dtMeshGEdge::~dtMeshGEdge() {
  }

  void dtMeshGEdge::operator()( dtGmshEdge * dtge) {
      ::meshGEdge()( dtge );
  }
}

