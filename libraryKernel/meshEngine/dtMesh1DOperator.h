#ifndef DTMESH1DOPERATOR_H
#define	DTMESH1DOPERATOR_H

#include "dtMeshOperator.h"

class QDomElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtGmshEdge;
  
  class dtMesh1DOperator : public dtMeshOperator {
  public:
    dt__class(dtMesh1DOperator, dtMeshOperator);    
    dtMesh1DOperator();
    virtual ~dtMesh1DOperator();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtMeshOperator * > const * const mO      
    );
    virtual void operator()( dtGmshEdge * dtge ) = 0;    
    static void copyMesh( dtGmshEdge * from, dtGmshEdge *to );
  private:

  };
}
#endif	/* DTMESH1DOPERATOR_H */

