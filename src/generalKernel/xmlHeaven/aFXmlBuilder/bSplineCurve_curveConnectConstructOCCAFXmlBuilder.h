#ifndef bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H
#define	bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_curveConnectConstructOCCAFXmlBuilder : public aFXmlBuilder {
  public:  
    dt__classOnlyName(bSplineCurve_curveConnectConstructOCCAFXmlBuilder);
    dt__classSelfCreate(bSplineCurve_curveConnectConstructOCCAFXmlBuilder);
    bSplineCurve_curveConnectConstructOCCAFXmlBuilder();
    virtual ~bSplineCurve_curveConnectConstructOCCAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF,  
      aFPtrVec * result 
    ) const; 
  };
}
#endif	/* bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H */

