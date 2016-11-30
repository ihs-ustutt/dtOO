#ifndef bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder_H
#define	bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder);
      bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder();
      virtual ~bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;
  };
}

#endif	/* bSplineSurface_bSplineCurveFillConstructOCCAGXmlBuilder_H */