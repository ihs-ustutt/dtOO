#ifndef map2dTo3dApproximateInMap3dTo3dAGXmlBuilder_H
#define	map2dTo3dApproximateInMap3dTo3dAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class map2dTo3dApproximateInMap3dTo3dAGXmlBuilder 
    : public aGXmlBuilder {
    public:
      dt__classOnlyName(map2dTo3dApproximateInMap3dTo3dAGXmlBuilder);      
      map2dTo3dApproximateInMap3dTo3dAGXmlBuilder();
      virtual ~map2dTo3dApproximateInMap3dTo3dAGXmlBuilder();
      virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
      ) const;      
    private:

  };
}

#endif	/* map2dTo3dApproximateInMap3dTo3dAGXmlBuilder_H */

