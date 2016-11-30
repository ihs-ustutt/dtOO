#include "dtMeshOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
  dtMeshOperator::dtMeshOperator() {
  }

  dtMeshOperator::~dtMeshOperator() {
  }

  void dtMeshOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO    
  ) {
    labelHandling::setLabel(
      qtXmlPrimitive::getAttributeStr("label", element ) 
    );
  }
}
