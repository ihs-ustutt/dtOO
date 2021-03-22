#ifndef surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H
#define	surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder);
      dt__classSelfCreate(surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder);
      surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder();
      virtual ~surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder();
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
#endif	/* surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H */

