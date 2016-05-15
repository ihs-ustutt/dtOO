#ifndef bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder_H
#define	bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder);
      bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder();
      virtual ~bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_straightExtentToSurfaceOCCAGXmlBuilder_H */

