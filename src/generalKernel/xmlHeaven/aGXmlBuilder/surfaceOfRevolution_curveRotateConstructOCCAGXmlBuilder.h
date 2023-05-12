#ifndef surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H
#define	surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result
      ) const;  
    private: 
      static bool _registrated;          
  };
}
#endif	/* surfaceOfRevolution_curveRotateConstructOCCAGXmlBuilder_H */

