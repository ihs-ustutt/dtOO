#include "dtMeshAndOrientGEdge.h"

#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>
#include "dtMeshOperatorFactory.h"

namespace dtOO {
  bool dtMeshAndOrientGEdge::_registrated 
  =
  dtMeshOperatorFactory::registrate(
    dt__tmpPtr(dtMeshAndOrientGEdge, new dtMeshAndOrientGEdge())
  );
 
 
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

