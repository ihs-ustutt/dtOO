#ifndef DTXMLPARSERFUNCTIONDECORATORINTERFACE_H
#define	DTXMLPARSERFUNCTIONDECORATORINTERFACE_H

#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;  
  
  class dtXmlParserFunctionDecoratorInterface {
  public:  
    dt__CLASSNAME(dtXmlParserFunctionDecoratorInterface);
    dtXmlParserFunctionDecoratorInterface();
    virtual ~dtXmlParserFunctionDecoratorInterface();
    virtual void buildPart(QDomElement const & toBuildP, 
                                          vectorHandling< constValue * > const * const cValP,
                                          vectorHandling< analyticFunction * > const * const depSFunP,
                                          vectorHandling< analyticFunction * > * sFunP) const = 0;
    virtual void buildPartCompound(QDomElement const & toBuildP, 
                                          vectorHandling< constValue * > const * const cValP,
                                          vectorHandling< analyticFunction * > const * const depSFunP,
                                          vectorHandling< analyticFunction * > * sFunP) const;    
  };
}
#endif	/* DTXMLPARSERFUNCTIONDECORATORINTERFACE_H */

