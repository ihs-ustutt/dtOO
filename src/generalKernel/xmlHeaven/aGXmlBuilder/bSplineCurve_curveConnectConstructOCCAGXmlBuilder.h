#ifndef bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H
#define	bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineCurve_curveConnectConstructOCCAGXmlBuilder : public aGXmlBuilder {
    public:  
      dt__classOnlyName(bSplineCurve_curveConnectConstructOCCAGXmlBuilder);
      dt__classSelfCreate(bSplineCurve_curveConnectConstructOCCAGXmlBuilder);
      bSplineCurve_curveConnectConstructOCCAGXmlBuilder();
      virtual ~bSplineCurve_curveConnectConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_curveConnectConstructOCCAGXmlBuilder_H */

