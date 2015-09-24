#include "dtMesh1DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>

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
}
