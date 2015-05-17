#ifndef sca5PointMeanlineAFXmlBuilder_H
#define	sca5PointMeanlineAFXmlBuilder_H

#include <dtXmlParserDecorator/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class sca5PointMeanlineAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(sca5PointMeanlineAFXmlBuilder);    
    sca5PointMeanlineAFXmlBuilder();
    virtual ~sca5PointMeanlineAFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* sca5PointMeanlineAFXmlBuilder_H */

