#ifndef bezierCurve_pointConstructOCCAGXmlBuilder_H
#define	bezierCurve_pointConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierCurve_pointConstructOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bezierCurve_pointConstructOCCAGXmlBuilder);
    dt__classSelfCreate(bezierCurve_pointConstructOCCAGXmlBuilder);
    bezierCurve_pointConstructOCCAGXmlBuilder();
    virtual ~bezierCurve_pointConstructOCCAGXmlBuilder();
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
#endif	/* bezierCurve_pointConstructOCCAGXmlBuilder_H */

