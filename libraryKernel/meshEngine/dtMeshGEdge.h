#ifndef dtMeshGEdge_H
#define	dtMeshGEdge_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh1DOperator.h"

namespace dtOO {
  class dtGmshEdge;
  
  class dtMeshGEdge : public dtMesh1DOperator {
  public:
    dt__class(dtMeshGEdge, dtMeshOperator);     
    dtMeshGEdge();
    dtMeshGEdge(const dtMeshGEdge& orig);
    virtual ~dtMeshGEdge();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtMeshOperator * > const * const mO
    );    
    virtual void operator()( dtGmshEdge * dtge );    
  };
}
#endif	/* dtMeshGEdge_H */

