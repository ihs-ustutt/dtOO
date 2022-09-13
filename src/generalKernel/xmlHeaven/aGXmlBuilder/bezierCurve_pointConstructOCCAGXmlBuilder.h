#ifndef bezierCurve_pointConstructOCCAGXmlBuilder_H
#define	bezierCurve_pointConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}
#endif	/* bezierCurve_pointConstructOCCAGXmlBuilder_H */

