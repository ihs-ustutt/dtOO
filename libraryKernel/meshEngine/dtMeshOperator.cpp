#include "dtMeshOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>

namespace dtOO {
  dtMeshOperator::dtMeshOperator() {
  }

  dtMeshOperator::dtMeshOperator(const dtMeshOperator& orig) {
    labelHandling::setLabel( orig.getLabel() );
  }

  dtMeshOperator::~dtMeshOperator() {
  }

  void dtMeshOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO    
  ) {
    labelHandling::setLabel(
      qtXmlPrimitive::getAttributeStr("label", element ) 
    );
    
    
  }
}
