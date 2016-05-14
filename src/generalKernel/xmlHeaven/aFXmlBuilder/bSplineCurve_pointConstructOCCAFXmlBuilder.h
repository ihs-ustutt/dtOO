#ifndef bSplineCurve_pointConstructOCCAFXmlBuilder_H
#define	bSplineCurve_pointConstructOCCAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class bSplineCurve_pointConstructOCCAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(bSplineCurve_pointConstructOCCAFXmlBuilder);    
    dt__classSelfCreate(bSplineCurve_pointConstructOCCAFXmlBuilder);    
    bSplineCurve_pointConstructOCCAFXmlBuilder();
    virtual ~bSplineCurve_pointConstructOCCAFXmlBuilder();
    virtual std::vector< std::string > factoryAlias( void ) const;        
    virtual void buildPart(
      ::QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* bSplineCurve_pointConstructOCCAFXmlBuilder_H */

