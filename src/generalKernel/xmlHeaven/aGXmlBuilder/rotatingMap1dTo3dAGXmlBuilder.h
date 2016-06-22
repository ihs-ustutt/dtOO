#ifndef rotatingMap1dTo3dAGXmlBuilder_H
#define	rotatingMap1dTo3dAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class rotatingMap1dTo3dAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(rotatingMap1dTo3dAGXmlBuilder);
    dt__classSelfCreate(rotatingMap1dTo3dAGXmlBuilder);
    rotatingMap1dTo3dAGXmlBuilder();
    virtual ~rotatingMap1dTo3dAGXmlBuilder();
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
#endif	/* rotatingMap1dTo3dAGXmlBuilder_H */

