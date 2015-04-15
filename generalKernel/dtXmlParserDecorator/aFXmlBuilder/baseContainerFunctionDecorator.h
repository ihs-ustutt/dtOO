#ifndef BASECONTAINERFUNCTIONDECORATOR_H
#define	BASECONTAINERFUNCTIONDECORATOR_H

#include "baseContainerFunctionDecorator.h"
#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class baseContainerFunctionDecorator : public dtXmlParserFunctionDecorator {
    dt__classOnlyName(baseContainerFunctionDecorator);
  public:
    baseContainerFunctionDecorator();
    virtual ~baseContainerFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;  
  };
}
#endif	/* BASECONTAINERFUNCTIONDECORATOR_H */

