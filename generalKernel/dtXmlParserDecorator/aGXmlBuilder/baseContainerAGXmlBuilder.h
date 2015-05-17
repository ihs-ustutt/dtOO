#ifndef baseContainerAGXmlBuilder_H
#define	baseContainerAGXmlBuilder_H

#include "baseContainerAGXmlBuilder.h"
#include <dtXmlParserDecorator/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  class vectorContainer;

  class baseContainerAGXmlBuilder : public aGXmlBuilder {
  public:
    dt__classOnlyName(baseContainerAGXmlBuilder);    
    baseContainerAGXmlBuilder();
    virtual ~baseContainerAGXmlBuilder();
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
#endif	/* baseContainerAGXmlBuilder_H */

