#ifndef SCA3POINTMEANLINEFUNCTIONDECORATOR_H
#define	SCA3POINTMEANLINEFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class sca3PointMeanlineFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:
    dt__CLASSNAME(sca3PointMeanlineFunctionDecorator);    
    sca3PointMeanlineFunctionDecorator();
    virtual ~sca3PointMeanlineFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* SCA3POINTMEANLINEFUNCTIONDECORATOR_H */

