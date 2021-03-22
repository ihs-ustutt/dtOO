#ifndef bSplineSurface_skinConstructOCCAGXmlBuilder_H
#define	bSplineSurface_skinConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_skinConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineSurface_skinConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineSurface_skinConstructOCCAGXmlBuilder);
      bSplineSurface_skinConstructOCCAGXmlBuilder();
      virtual ~bSplineSurface_skinConstructOCCAGXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild,
        baseContainer * const bC,
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const; 
    private: 
      static bool _registrated;            
  };
}
#endif	/* bSplineSurface_skinConstructOCCAGXmlBuilder_H */

