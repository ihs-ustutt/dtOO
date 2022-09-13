#ifndef bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder_H
#define	bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(
        bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder
      );    
      dt__classSelfCreate(
        bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder
      );
      bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder();
      virtual ~bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuildP,
        baseContainer * const bC,
        lvH_constValue const * const cV, 
        lvH_analyticFunction const * const aF,
        lvH_analyticFunction * result
      ) const;
    private: 
      static bool _registrated;      
    };
}
#endif	/* bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder_H */

