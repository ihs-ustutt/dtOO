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
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO    
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }
  
  void dtMesh1DOperator::copyMesh( dtGmshEdge * from, dtGmshEdge * to) {
    ::copyMesh( (::GEdge*) from, (::GEdge*) to, to->masterOrientation );
  }

}
