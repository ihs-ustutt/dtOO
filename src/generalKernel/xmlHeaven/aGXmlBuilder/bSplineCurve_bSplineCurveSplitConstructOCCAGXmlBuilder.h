#ifndef bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder_H
#define	bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder 
    : public aGXmlBuilder {
  public:  
    dt__classOnlyName(bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder);
    dt__classSelfCreate(bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder);
    bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder();
    virtual ~bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuild,
      baseContainer * const bC,
      cVPtrVec const * const cV,  
      aFPtrVec const * const aF,  
      aGPtrVec const * const aG,
      aGPtrVec * result 
    ) const;
  };
}
#endif	/* bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder_H */

