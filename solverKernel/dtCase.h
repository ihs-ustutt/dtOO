#ifndef DTCASE_H
#define	DTCASE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class dtCase : public labelHandling,
                 public optionHandling {
  public:
    dt__CLASSNAME(dtCase);    
    dtCase();
    virtual ~dtCase();
    virtual void init( 
      QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtCase * > const * const dC
    );    
    virtual void apply(void) = 0;    
  private:

  };
}
#endif	/* SOLVERCASE_H */
