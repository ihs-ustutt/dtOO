#ifndef dtMeshFixedBoundaryElementGEdge_H
#define	dtMeshFixedBoundaryElementGEdge_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh1DOperator.h"

namespace dtOO {
  class dtGmshEdge;
  class scaOneD;
  
  class dtMeshFixedBoundaryElementGEdge : public dtMesh1DOperator {
    public:
      dt__class(dtMeshFixedBoundaryElementGEdge, dtMeshOperator);     
      dtMeshFixedBoundaryElementGEdge();
      virtual ~dtMeshFixedBoundaryElementGEdge();
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
    private:
      dt__pH(scaOneD const) _grading;
      int _typeTransfinite;
  };
}
#endif	/* dtMeshFixedBoundaryElementGEdge_H */

