#ifndef DTMESHOPERATOR_H
#define	DTMESHOPERATOR_H

#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <interfaceHeaven/vectorHandling.h>

class QDomElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  
  class dtMeshOperator : public labelHandling, public optionHandling {
  public:
    dt__classOnlyName(dtMeshOperator);    
    dtMeshOperator();
    virtual ~dtMeshOperator();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtMeshOperator * > const * const mO
    );
  private:

  };
}
#endif	/* DTMESHOPERATOR_H */

