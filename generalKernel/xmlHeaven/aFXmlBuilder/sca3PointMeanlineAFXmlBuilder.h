#ifndef sca3PointMeanlineAFXmlBuilder_H
#define	sca3PointMeanlineAFXmlBuilder_H

#include <xmlHeaven/aFXmlBuilder.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class sca3PointMeanlineAFXmlBuilder : public aFXmlBuilder {
  public:
    dt__classOnlyName(sca3PointMeanlineAFXmlBuilder);    
    sca3PointMeanlineAFXmlBuilder();
    virtual ~sca3PointMeanlineAFXmlBuilder();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* sca3PointMeanlineAFXmlBuilder_H */
