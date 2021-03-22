#ifndef bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H
#define	bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder);
      bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder();
      virtual ~bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder();
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

#endif	/* bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H */