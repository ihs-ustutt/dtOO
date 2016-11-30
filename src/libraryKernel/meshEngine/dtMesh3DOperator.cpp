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
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO  
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }
}
