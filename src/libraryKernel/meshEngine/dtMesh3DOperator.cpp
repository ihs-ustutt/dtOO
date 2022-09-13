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
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO  
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }
}
