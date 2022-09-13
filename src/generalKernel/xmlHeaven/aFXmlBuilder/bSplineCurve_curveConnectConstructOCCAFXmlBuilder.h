#ifndef bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H
#define	bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_curveConnectConstructOCCAFXmlBuilder : public aFXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_curveConnectConstructOCCAFXmlBuilder);
      dt__classSelfCreate(bSplineCurve_curveConnectConstructOCCAFXmlBuilder);
      bSplineCurve_curveConnectConstructOCCAFXmlBuilder();
      virtual ~bSplineCurve_curveConnectConstructOCCAFXmlBuilder();
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
#endif	/* bSplineCurve_curveConnectConstructOCCAFXmlBuilder_H */

