#ifndef bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder_H
#define	bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder);
    dt__classSelfCreate(bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder);
    bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder();
    virtual ~bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder_H */