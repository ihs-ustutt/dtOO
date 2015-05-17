#ifndef rotatingMap2dTo3dDecorator_H
#define	rotatingMap2dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class rotatingMap2dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(rotatingMap2dTo3dDecorator);
    rotatingMap2dTo3dDecorator();
    virtual ~rotatingMap2dTo3dDecorator();
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
#endif	/* rotatingMap2dTo3dDecorator_H */

