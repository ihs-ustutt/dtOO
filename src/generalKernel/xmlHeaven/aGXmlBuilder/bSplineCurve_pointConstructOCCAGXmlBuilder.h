#ifndef bSplineCurve_pointConstructOCCAGXmlBuilder_H
#define	bSplineCurve_pointConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_pointConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_pointConstructOCCAGXmlBuilder);
      bSplineCurve_pointConstructOCCAGXmlBuilder();
      virtual ~bSplineCurve_pointConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_pointConstructOCCAGXmlBuilder_H */

