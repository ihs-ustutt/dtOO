#ifndef bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder_H
#define	bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder_H

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
        cVPtrVec const * const cV, 
        aFPtrVec const * const aF,
        aFPtrVec * result
      ) const;
    };
}
#endif	/* bSplineCurve_normalOffsetGeomCurveOCCAFXmlBuilder_H */

