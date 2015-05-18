#ifndef scaMuParserTwoDAFXmlBuilder_H
#define	scaMuParserTwoDAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaMuParserTwoDAFXmlBuilder : public aFXmlBuilder {
  public:  
    dt__classOnlyName(scaMuParserTwoDAFXmlBuilder);
    scaMuParserTwoDAFXmlBuilder();
    virtual ~scaMuParserTwoDAFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* scaMuParserTwoDAFXmlBuilder_H */