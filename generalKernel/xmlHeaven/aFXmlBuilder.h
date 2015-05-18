#ifndef aFXmlBuilder_H
#define	aFXmlBuilder_H

#include "aFXmlBuilder.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class baseContainer;  
  class constValue;
  class analyticFunction;

  class aFXmlBuilder {
  public:  
    dt__classOnlyName(aFXmlBuilder);
    aFXmlBuilder();
    ~aFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuild, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticFunction * > * result
    ) const = 0;
    virtual void buildPartCompound(
      QDomElement const & toBuild, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticFunction * > * result
    ) const;     
  };
}
#endif	/* aFXmlBuilder_H */

