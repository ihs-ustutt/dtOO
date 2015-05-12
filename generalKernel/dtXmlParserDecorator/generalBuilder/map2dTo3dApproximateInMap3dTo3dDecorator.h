#ifndef map2dTo3dApproximateInMap3dTo3dDecorator_H
#define	map2dTo3dApproximateInMap3dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class map2dTo3dApproximateInMap3dTo3dDecorator 
    : public dtXmlParserDecorator {
    public:
      dt__classOnlyName(map2dTo3dApproximateInMap3dTo3dDecorator);      
      map2dTo3dApproximateInMap3dTo3dDecorator();
      virtual ~map2dTo3dApproximateInMap3dTo3dDecorator();
      virtual void buildPart(
        QDomElement ** toBuildP,
        baseContainer * const bC,
        vectorHandling< constValue * > const * const cV,  
        vectorHandling< analyticFunction * > const * const aF,  
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< analyticGeometry * > * rAG 
      ) const;      
    private:

  };
}

#endif	/* map2dTo3dApproximateInMap3dTo3dDecorator_H */

