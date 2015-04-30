#ifndef bSplineCurve_straightExtentToSurfaceOCCDecorator_H
#define	bSplineCurve_straightExtentToSurfaceOCCDecorator_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_straightExtentToSurfaceOCCDecorator : public dtXmlParserDecorator {
    public:  
      dt__classOnlyName(bSplineCurve_straightExtentToSurfaceOCCDecorator);
      bSplineCurve_straightExtentToSurfaceOCCDecorator();
      virtual ~bSplineCurve_straightExtentToSurfaceOCCDecorator();
      virtual void buildPart(
        QDomElement ** toBuildP,
        baseContainer * const bC,
        vectorHandling< constValue * > const * const cV,  
        vectorHandling< analyticFunction * > const * const aF,  
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< analyticGeometry * > * rAG 
      ) const; 
  };
}
#endif	/* bSplineCurve_straightExtentToSurfaceOCCDecorator_H */

