#ifndef bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder_H
#define	bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(
        bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder
      );    
      dt__classSelfCreate(
        bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder
      );
      bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder();
      virtual ~bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuild, 
        baseContainer * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aFPtrVec * result
      ) const;
    private: 
      static bool _registrated;      
    };
}
#endif	/* bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder_H */

