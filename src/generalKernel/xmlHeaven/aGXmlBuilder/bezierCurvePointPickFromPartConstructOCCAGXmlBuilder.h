#ifndef bezierCurvePointPickFromPartConstructOCCAGXmlBuilder_H
#define	bezierCurvePointPickFromPartConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bezierCurvePointPickFromPartConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bezierCurvePointPickFromPartConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bezierCurvePointPickFromPartConstructOCCAGXmlBuilder);
      bezierCurvePointPickFromPartConstructOCCAGXmlBuilder();
      virtual ~bezierCurvePointPickFromPartConstructOCCAGXmlBuilder();
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

#endif	/* bezierCurvePointPickFromPartConstructOCCAGXmlBuilder_H */

