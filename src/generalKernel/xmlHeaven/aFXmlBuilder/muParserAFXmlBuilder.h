#ifndef muParserAFXmlBuilder_H
#define	muParserAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class muParserAFXmlBuilder : public aFXmlBuilder {
  public:  
    dt__classOnlyName(muParserAFXmlBuilder);
    dt__classSelfCreate(muParserAFXmlBuilder);
    muParserAFXmlBuilder();
    virtual ~muParserAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* muParserAFXmlBuilder_H */