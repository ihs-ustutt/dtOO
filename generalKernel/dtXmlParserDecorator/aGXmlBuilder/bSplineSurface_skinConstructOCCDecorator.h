#ifndef BPLINESURFACE_SKINCONSTRUCTOCCDECORATOR_H
#define	BPLINESURFACE_SKINCONSTRUCTOCCDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserDecorator.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_skinConstructOCCDecorator : public dtXmlParserDecorator {
  public:  
    dt__classOnlyName(bSplineSurface_skinConstructOCCDecorator);
    bSplineSurface_skinConstructOCCDecorator();
    virtual ~bSplineSurface_skinConstructOCCDecorator();
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
#endif	/* BPLINESURFACE_SKINCONSTRUCTOCCDECORATOR_H */

