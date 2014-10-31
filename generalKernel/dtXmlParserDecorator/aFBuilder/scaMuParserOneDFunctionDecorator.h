#ifndef SCAMUPARSERONEDFUNCTIONDECORATOR_H
#define	SCAMUPARSERONEDFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;

  class scaMuParserOneDFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:  
    dt__CLASSNAME(scaMuParserOneDFunctionDecorator);
    scaMuParserOneDFunctionDecorator();
    virtual ~scaMuParserOneDFunctionDecorator();
    virtual void buildPart(QDomElement const & toBuildP, 
                                          vectorHandling< constValue * > const * const cValP, 
                                          vectorHandling< analyticFunction * > const * const depSFunP,
                                          vectorHandling< analyticFunction * > * sFunP) const;
  };
}
#endif	/* SCAMUPARSERONEDFUNCTIONDECORATOR_H */