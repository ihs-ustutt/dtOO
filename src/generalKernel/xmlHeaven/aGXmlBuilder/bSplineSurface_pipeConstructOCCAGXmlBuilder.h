#ifndef bSplineSurface_pipeConstructOCCAGXmlBuilder_H
#define	bSplineSurface_pipeConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_pipeConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bSplineSurface_pipeConstructOCCAGXmlBuilder);
    dt__classSelfCreate(bSplineSurface_pipeConstructOCCAGXmlBuilder);
    bSplineSurface_pipeConstructOCCAGXmlBuilder();
    virtual ~bSplineSurface_pipeConstructOCCAGXmlBuilder();
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
#endif	/* bSplineSurface_pipeConstructOCCAGXmlBuilder_H */

