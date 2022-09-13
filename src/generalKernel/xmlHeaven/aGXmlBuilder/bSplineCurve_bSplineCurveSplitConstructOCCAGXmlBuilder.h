#ifndef bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder_H
#define	bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,  
        lvH_analyticFunction const * const aF,  
        lvH_analyticGeometry const * const aG,
        lvH_analyticGeometry * result 
      ) const;
    private: 
      static bool _registrated;            
  };
}
#endif	/* bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder_H */

