#ifndef DTMESHOPERATOR_H
#define	DTMESHOPERATOR_H

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
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      labeledVectorHandling< dtMeshOperator * > const * const mO
    );
  private:

  };
}
#endif	/* DTMESHOPERATOR_H */

