#include "dtMesh2DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
  dtMesh2DOperator::dtMesh2DOperator() {
  }

  dtMesh2DOperator::dtMesh2DOperator(const dtMesh2DOperator& orig) {
  }

  dtMesh2DOperator::~dtMesh2DOperator() {
  }

  void dtMesh2DOperator::init(
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
}
