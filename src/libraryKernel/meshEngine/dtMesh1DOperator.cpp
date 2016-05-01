#include "dtMesh1DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>
void copyMesh(GEdge *from, GEdge *to, int direction);

namespace dtOO {
  dtMesh1DOperator::dtMesh1DOperator() {
  }

  dtMesh1DOperator::~dtMesh1DOperator() {
  }

  void dtMesh1DOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO    
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }
  
  void dtMesh1DOperator::copyMesh( dtGmshEdge * from, dtGmshEdge * to) {
    ::copyMesh( (::GEdge*) from, (::GEdge*) to, to->masterOrientation );
  }

}
