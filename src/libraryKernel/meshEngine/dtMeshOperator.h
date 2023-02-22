#ifndef DTMESHOPERATOR_H
#define	DTMESHOPERATOR_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/labelHandling.h>
#include <interfaceHeaven/optionHandling.h>
#include <mainConceptFwd.h>

class QDomElement;

namespace dtOO {
  class baseContainer;
  
  class dtMeshOperator : public labelHandling, public optionHandling {
  public:
    dt__classOnlyName(dtMeshOperator);    
    dtMeshOperator();
    virtual ~dtMeshOperator();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      lvH_constValue const * const cV,
      lvH_analyticFunction const * const aF,
      lvH_analyticGeometry const * const aG,
      lvH_boundedVolume const * const bV,
      lvH_dtMeshOperator const * const mO
    );
  private:

  };
}
#endif	/* DTMESHOPERATOR_H */

