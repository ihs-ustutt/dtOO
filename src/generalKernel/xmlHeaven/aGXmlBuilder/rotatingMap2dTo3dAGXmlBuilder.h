#ifndef rotatingMap2dTo3dAGXmlBuilder_H
#define	rotatingMap2dTo3dAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class rotatingMap2dTo3dAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(rotatingMap2dTo3dAGXmlBuilder);
    rotatingMap2dTo3dAGXmlBuilder();
    virtual ~rotatingMap2dTo3dAGXmlBuilder();
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
#endif	/* rotatingMap2dTo3dAGXmlBuilder_H */

