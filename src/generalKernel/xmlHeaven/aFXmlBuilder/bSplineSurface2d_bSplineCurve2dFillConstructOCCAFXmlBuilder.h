#ifndef bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder_H
#define	bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder_H

#include <dtOOTypeDef.h>

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder 
    : public aFXmlBuilder {
    public:
      dt__classOnlyName(bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder);    
      dt__classSelfCreate(bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder);    
      bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder();
      virtual ~bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder();
      virtual void buildPart(
        ::QDomElement const & toBuildP, 
        baseContainer * const bC,
        lvH_constValue const * const cValP, 
        lvH_analyticFunction const * const depSFunP,
        lvH_analyticFunction * sFunP
      ) const;
    private: 
      static bool _registrated;      
  };
}
#endif	/* bSplineSurface2d_bSplineCurve2dFillConstructOCCAFXmlBuilder_H */

