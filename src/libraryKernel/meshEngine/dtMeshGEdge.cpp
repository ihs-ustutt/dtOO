#include "dtMeshGEdge.h"

#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>

namespace dtOO {
  dtMeshGEdge::dtMeshGEdge() : dtMesh1DOperator() {
  }

  dtMeshGEdge::dtMeshGEdge(const dtMeshGEdge& orig) : dtMesh1DOperator(orig) {
  }

  dtMeshGEdge::~dtMeshGEdge() {
  } 

  void dtMeshGEdge::operator()( dtGmshEdge * dtge) {
      ::meshGEdge()( dtge );
  }
}

