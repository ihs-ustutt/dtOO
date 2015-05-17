#ifndef infinityMap3dTo3dDecorator_H
#define	infinityMap3dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class infinityMap3dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(infinityMap3dTo3dDecorator);
    infinityMap3dTo3dDecorator();
    virtual ~infinityMap3dTo3dDecorator();
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
#endif	/* infinityMap3dTo3dDecorator_H */

