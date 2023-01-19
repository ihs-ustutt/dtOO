#ifndef aFJsonBuilder_H
#define	aFJsonBuilder_H

#include <dtOOTypeDef.h>

#include "aFJsonBuilder.h"
#include <mainConceptFwd.h>
#include <interfaceHeaven/labeledVectorHandling.h>
#include <logMe/dtMacros.h>
#include <jsonHeaven/jsonPrimitive.h>

namespace dtOO {
  class baseContainer;  
  class constValue;
  class analyticFunction;

  class aFJsonBuilder {
    public:  
      dt__classOnlyName(aFJsonBuilder);
      aFJsonBuilder();
      ~aFJsonBuilder();
      virtual lvH_analyticFunction buildPart(
        jsonPrimitive const & config, 
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF
      ) const = 0;
  };
}
#endif	/* aFJsonBuilder_H */

