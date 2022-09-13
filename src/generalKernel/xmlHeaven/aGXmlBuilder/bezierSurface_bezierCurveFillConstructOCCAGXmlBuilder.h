#ifndef bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H
#define	bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;
    private: 
      static bool _registrated;          
  };
}

#endif	/* bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder_H */