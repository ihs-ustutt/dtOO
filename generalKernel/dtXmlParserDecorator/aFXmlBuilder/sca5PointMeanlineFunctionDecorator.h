#ifndef SCA5POINTMEANLINEFUNCTIONDECORATOR_H
#define	SCA5POINTMEANLINEFUNCTIONDECORATOR_H

#include <dtXmlParserDecorator/dtXmlParserFunctionDecorator.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class analyticFunction;
  class constValue;
  class baseContainer;
  
  class sca5PointMeanlineFunctionDecorator : public dtXmlParserFunctionDecorator {
  public:
    dt__CLASSNAME(sca5PointMeanlineFunctionDecorator);    
    sca5PointMeanlineFunctionDecorator();
    virtual ~sca5PointMeanlineFunctionDecorator();
    virtual void buildPart(
      QDomElement const & toBuildP, 
      baseContainer * const bC,
      vectorHandling< constValue * > const * const cValP, 
      vectorHandling< analyticFunction * > const * const depSFunP,
      vectorHandling< analyticFunction * > * sFunP
    ) const;
  };
}
#endif	/* SCA5POINTMEANLINEFUNCTIONDECORATOR_H */

