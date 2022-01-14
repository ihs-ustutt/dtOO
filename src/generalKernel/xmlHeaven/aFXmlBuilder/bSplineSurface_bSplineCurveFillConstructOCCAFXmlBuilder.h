#ifndef bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder_H
#define	bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder);    
      dt__classSelfCreate(bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder);    
      bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder();
      virtual ~bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuildP, 
        baseContainer * const bC,
        cVPtrVec const * const cValP, 
        aFPtrVec const * const depSFunP,
        aFPtrVec * sFunP
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* bSplineSurface_bSplineCurveFillConstructOCCAFXmlBuilder_H */

