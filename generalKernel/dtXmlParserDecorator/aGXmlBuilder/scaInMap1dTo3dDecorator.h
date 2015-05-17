#ifndef scaInMap1dTo3dDecorator_H
#define	scaInMap1dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaInMap1dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(scaInMap1dTo3dDecorator);
    scaInMap1dTo3dDecorator();
    virtual ~scaInMap1dTo3dDecorator();
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
#endif	/* scaInMap1dTo3dDecorator_H */

