#ifndef dtMeshFreeGradingGEdge_H
#define	dtMeshFreeGradingGEdge_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshGEdge.h"

namespace dtOO {
  class dtGmshEdge;
  class scaOneD;
  
  class dtMeshFreeGradingGEdge : public dtMeshGEdge {
    public:
      dt__class(dtMeshFreeGradingGEdge, dtMeshOperator);     
      dtMeshFreeGradingGEdge();
      virtual ~dtMeshFreeGradingGEdge();
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
      dt__pVH(scaOneD) _grading;
      std::vector< int > _typeTransfinite;
      std::map< int, scaOneD * > _gradingInt;
  };
}
#endif	/* dtMeshFreeGradingGEdge_H */

