#ifndef baseContainerAFXmlBuilder_H
#define	baseContainerAFXmlBuilder_H

#include "baseContainerAFXmlBuilder.h"
#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;

  class baseContainerAFXmlBuilder : public aFXmlBuilder {
    dt__classOnlyName(baseContainerAFXmlBuilder);
  public:
    baseContainerAFXmlBuilder();
    virtual ~baseContainerAFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuildP,
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP,  
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;  
  };
}
#endif	/* baseContainerAFXmlBuilder_H */

