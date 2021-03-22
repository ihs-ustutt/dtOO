#ifndef bezierCurvePointPickFromPartConstructOCCAGXmlBuilder_H
#define	bezierCurvePointPickFromPartConstructOCCAGXmlBuilder_H

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
        cVPtrVec const * const cV,  
        aFPtrVec const * const aF,  
        aGPtrVec const * const aG,
        aGPtrVec * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}

#endif	/* bezierCurvePointPickFromPartConstructOCCAGXmlBuilder_H */

