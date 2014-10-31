#ifndef DTXMLPARSERFUNCTIONDECORATOR_H
#define	DTXMLPARSERFUNCTIONDECORATOR_H

#include "dtXmlParserFunctionDecorator.h"
#include "dtXmlParserFunctionDecoratorInterface.h"
#include "dtXmlParserBase.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class constValue;
  class analyticFunction;

  class dtXmlParserFunctionDecorator : public dtXmlParserFunctionDecoratorInterface, public dtXmlParserBase {
  public:  
    dt__CLASSNAME(dtXmlParserFunctionDecorator);
    dtXmlParserFunctionDecorator();
    ~dtXmlParserFunctionDecorator();
    virtual void buildPart(QDomElement const & toBuildP, 
                                          vectorHandling< constValue * > const * const cValP,
                                          vectorHandling< analyticFunction * > const * const depSFunP,
                                          vectorHandling< analyticFunction * > * sFunP) const;
    virtual void buildPartCompound(QDomElement const & toBuildP, 
                                          vectorHandling< constValue * > const * const cValP, 
                                          vectorHandling< analyticFunction * > const * const depSFunP,
                                          vectorHandling< analyticFunction * > * sFunP) const;     
  };
}
#endif	/* DTXMLPARSERFUNCTIONDECORATOR_H */

