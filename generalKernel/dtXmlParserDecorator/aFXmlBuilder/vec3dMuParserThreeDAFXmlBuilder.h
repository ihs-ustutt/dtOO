#ifndef vec3dMuParserThreeDAFXmlBuilder_H
#define	vec3dMuParserThreeDAFXmlBuilder_H

#include <dtXmlParserDecorator/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class vec3dMuParserThreeDAFXmlBuilder : public aFXmlBuilder {
  public:  
    dt__classOnlyName(vec3dMuParserThreeDAFXmlBuilder);
    vec3dMuParserThreeDAFXmlBuilder();
    virtual ~vec3dMuParserThreeDAFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* vec3dMuParserThreeDAFXmlBuilder_H */