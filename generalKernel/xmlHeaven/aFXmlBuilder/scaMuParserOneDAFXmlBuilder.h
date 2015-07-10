#ifndef scaMuParserOneDAFXmlBuilder_H
#define	scaMuParserOneDAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class scaMuParserOneDAFXmlBuilder : public aFXmlBuilder {
  public:  
    dt__classOnlyName(scaMuParserOneDAFXmlBuilder);
    scaMuParserOneDAFXmlBuilder();
    virtual ~scaMuParserOneDAFXmlBuilder();
    virtual void buildPart(
      ::QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* scaMuParserOneDAFXmlBuilder_H */