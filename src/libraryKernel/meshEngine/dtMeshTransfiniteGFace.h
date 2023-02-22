#ifndef dtMeshTransfiniteGFace_H
#define	dtMeshTransfiniteGFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMeshTransfinite2DOperator.h"
#include <interfaceHeaven/twoDArrayHandling.h>

class GFace;
class MVertex;

namespace dtOO {
  class dtGmshFace;
  
  class dtMeshTransfiniteGFace : public dtMeshTransfinite2DOperator {
    public:
      dt__class(dtMeshTransfiniteGFace, dtMeshOperator);     
      dtMeshTransfiniteGFace();
      dtMeshTransfiniteGFace(
        const dtMeshTransfiniteGFace& orig
      );
      virtual ~dtMeshTransfiniteGFace();
      void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO      
      ); 
      void operator()( dtGmshFace * dtgr );
    private:
      twoDArrayHandling< dtPoint2 > correctConstV( 
        dtGmshFace const * const dtgf, twoDArrayHandling< dtPoint2 > pUV,
        dtInt const & pos, dtInt const & nSteps, 
        std::vector< double > lengths_i, double const & L_i
      ) const;
      static twoDArrayHandling< dtPoint2 > linearInterpolateU( 
        twoDArrayHandling< dtPoint2 > pUV, dtInt const & vStart, dtInt const & vEnd
      );
    private:
//      std::vector< dtReal > _uCorrPos;
//      dtInt _uCorrSteps;
      std::vector< dtReal > _vCorrPos;
      dtInt _vCorrSteps;
  };
}
#endif	/* dtMeshTransfiniteGFace_H */

