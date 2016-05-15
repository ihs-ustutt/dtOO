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
      vectorHandling< constValue * > const * const cV,  
      vectorHandling< analyticFunction * > const * const aF,  
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< analyticGeometry * > * result 
    ) const;
  };
}

#endif	/* bezierCurvePointPickFromPartConstructOCCAGXmlBuilder_H */

