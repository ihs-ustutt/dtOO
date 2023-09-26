#include "aFJsonBuilder.h"
#include <logMe/logMe.h>
#include <baseContainerHeaven/baseContainer.h>

#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/labeledVectorHandling.h>

namespace dtOO {
  aFJsonBuilder::aFJsonBuilder() {
  }

  aFJsonBuilder::~aFJsonBuilder() {
  }

  lvH_analyticFunction aFJsonBuilder::buildPart( 
    jsonPrimitive const & config 
  ) const {
    return this->buildPart( config, NULL, NULL, NULL); 
  }
}
