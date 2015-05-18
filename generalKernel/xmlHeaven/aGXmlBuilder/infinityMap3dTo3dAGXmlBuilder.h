#ifndef infinityMap3dTo3dAGXmlBuilder_H
#define	infinityMap3dTo3dAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class infinityMap3dTo3dAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(infinityMap3dTo3dAGXmlBuilder);
    infinityMap3dTo3dAGXmlBuilder();
    virtual ~infinityMap3dTo3dAGXmlBuilder();
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
#endif	/* infinityMap3dTo3dAGXmlBuilder_H */

