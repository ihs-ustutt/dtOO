#ifndef DTXMLPARSERFUNCTIONDECORATOR_H
#define	DTXMLPARSERFUNCTIONDECORATOR_H

#include "dtXmlParserFunctionDecorator.h"
#include "dtXmlParserBase.h"
#include <interfaceHeaven/vectorHandling.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class constValue;
  class analyticFunction;

  class dtXmlParserFunctionDecorator : public dtXmlParserBase {
  public:  
    dt__CLASSNAME(dtXmlParserFunctionDecorator);
    dtXmlParserFunctionDecorator();
    ~dtXmlParserFunctionDecorator();
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
#endif	/* DTXMLPARSERFUNCTIONDECORATOR_H */

