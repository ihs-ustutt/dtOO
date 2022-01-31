#ifndef bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H
#define	bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_curveConnectConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_curveConnectConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_curveConnectConstructOCCAGXmlBuilder);
      bSplineCurve_curveConnectConstructOCCAGXmlBuilder();
      virtual ~bSplineCurve_curveConnectConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H */

