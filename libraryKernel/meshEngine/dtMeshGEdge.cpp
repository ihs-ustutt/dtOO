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

  void dtMeshGEdge::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh1DOperator::init(element, bC, cV, aF, aG, bV, mO);
  }
    

  void dtMeshGEdge::operator()( dtGmshEdge * dtge) {
      ::meshGEdge()( dtge );
  }
}

