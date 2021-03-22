#ifndef bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H
#define	bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder);
      bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder();
      virtual ~bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H */

