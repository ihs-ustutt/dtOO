#ifndef dtMeshFreeGradingGEdge_H
#define	dtMeshFreeGradingGEdge_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO      
      );
      virtual void operator()( dtGmshEdge * dtge );  
    private:
      dt__vH(scaOneD const *) _grading;
      std::vector< dtInt > _typeTransfinite;
      std::map< int, scaOneD const * > _gradingInt;
      dtReal _tol;
      dtInt _maxSmooth;
  };
}
#endif	/* dtMeshFreeGradingGEdge_H */
