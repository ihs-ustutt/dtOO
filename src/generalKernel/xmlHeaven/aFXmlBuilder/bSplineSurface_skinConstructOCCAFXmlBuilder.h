#ifndef VEC3DSURFACETWODAFXmlBuilder_H
#define	VEC3DSURFACETWODAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class bSplineSurface_skinConstructOCCAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(bSplineSurface_skinConstructOCCAFXmlBuilder);   
    dt__classSelfCreate(bSplineSurface_skinConstructOCCAFXmlBuilder);     
    bSplineSurface_skinConstructOCCAFXmlBuilder();
    virtual ~bSplineSurface_skinConstructOCCAFXmlBuilder();
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
#endif	/* VEC3DSURFACETWODAFXmlBuilder_H */

