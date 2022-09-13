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
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO    
  ) {
    labelHandling::setLabel(
      qtXmlPrimitive::getAttributeStr("label", element ) 
    );
    optionHandling::init( element, bC, cV, aF, aG );
  }
}
