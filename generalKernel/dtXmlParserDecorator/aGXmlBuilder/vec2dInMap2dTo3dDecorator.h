#ifndef vec2dInMap2dTo3dDecorator_H
#define	vec2dInMap2dTo3dDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec2dInMap2dTo3dDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(vec2dInMap2dTo3dDecorator);
    vec2dInMap2dTo3dDecorator();
    virtual ~vec2dInMap2dTo3dDecorator();
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
#endif	/* vec2dInMap2dTo3dDecorator_H */

