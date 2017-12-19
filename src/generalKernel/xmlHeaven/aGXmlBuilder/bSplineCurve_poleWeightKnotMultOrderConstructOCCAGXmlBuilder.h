#ifndef bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder_H
#define	bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder_H

#include <xmlHeaven/aGXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticGeometry;
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder 
    : public aGXmlBuilder {
    public:
      dt__classOnlyName(
        bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder
      );    
      dt__classSelfCreate(
        bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder
      );
      bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder();
      virtual ~bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder();
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
#endif	/* bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder_H */

