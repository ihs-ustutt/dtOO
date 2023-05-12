#ifndef dtMeshFreeGradingGEdge_H
#define	dtMeshFreeGradingGEdge_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshGEdge.h"

namespace dtOO {
  class dtGmshEdge;
  class scaOneD;

//! Mesh an edge with a predefined grading.
/*! 
 */
  class dtMeshFreeGradingGEdge : public dtMeshGEdge {
    public:
      dt__class(dtMeshFreeGradingGEdge, dtMeshOperator);     
      dt__classSelfCreate(dtMeshFreeGradingGEdge);
      dtMeshFreeGradingGEdge();
      virtual ~dtMeshFreeGradingGEdge();
      /*! 
       * @param jE JSON initialization.
       *          - typeTransfinite  : std::vector< dtInt >
       *          - gradingFunctions : std::vector< analyticFunction * >
       *          - _tolerance       : dtReal
       *          - _nSmoothSteps    : dtInt
       */
      virtual void jInit(
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );
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
      std::map< int, scaOneD const * > _gradingInt;
    private:
      static bool _registrated;
  };
}
#endif	/* dtMeshFreeGradingGEdge_H */
