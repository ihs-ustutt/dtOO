#ifndef makeCompoundAGXmlBuilder_H
#define	makeCompoundAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class makeCompoundAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(makeCompoundAGXmlBuilder);
    makeCompoundAGXmlBuilder();
    virtual ~makeCompoundAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;       
  };
}
#endif	/* makeCompoundAGXmlBuilder_H */

