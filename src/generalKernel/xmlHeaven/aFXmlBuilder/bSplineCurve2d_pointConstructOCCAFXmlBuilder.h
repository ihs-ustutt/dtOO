#ifndef bSplineCurve2d_pointConstructOCCAFXmlBuilder_H
#define	bSplineCurve2d_pointConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve2d_pointConstructOCCAFXmlBuilder : public aFXmlBuilder {
    public:
      dt__classOnlyName(bSplineCurve2d_pointConstructOCCAFXmlBuilder);    
      dt__classSelfCreate(bSplineCurve2d_pointConstructOCCAFXmlBuilder);    
      bSplineCurve2d_pointConstructOCCAFXmlBuilder();
      virtual ~bSplineCurve2d_pointConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild, 
        baseContainer * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticFunction * result
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* bSplineCurve2d_pointConstructOCCAFXmlBuilder_H */

