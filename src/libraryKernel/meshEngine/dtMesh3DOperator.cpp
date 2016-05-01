#include "dtMesh3DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
  dtMesh3DOperator::dtMesh3DOperator() {
  }

  dtMesh3DOperator::dtMesh3DOperator(const dtMesh3DOperator& orig) {
  }

  dtMesh3DOperator::~dtMesh3DOperator() {
  }

  void dtMesh3DOperator::init(
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
