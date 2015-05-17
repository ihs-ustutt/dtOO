#ifndef BASECONTAINERDECORATOR_H
#define	BASECONTAINERDECORATOR_H

#include "baseContainerDecorator.h"
#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class vectorContainer;

  class baseContainerDecorator : public dtXmlParserDecorator {
  public:
    dt__classOnlyName(baseContainerDecorator);    
    baseContainerDecorator();
    virtual ~baseContainerDecorator();
    virtual void buildPart(
      QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;  
  };
}
#endif	/* BASECONTAINERDECORATOR_H */

