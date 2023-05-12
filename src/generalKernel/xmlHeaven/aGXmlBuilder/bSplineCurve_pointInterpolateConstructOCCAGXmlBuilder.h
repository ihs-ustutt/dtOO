#ifndef bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H
#define	bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder);
      bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder();
      virtual ~bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder_H */

