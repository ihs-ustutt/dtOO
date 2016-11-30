#ifndef dtMeshTransfiniteGFace_H
#define	dtMeshTransfiniteGFace_H

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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        labeledVectorHandling< dtMeshOperator * > const * const mO      
      ); 
      void operator()( dtGmshFace * dtgr );
    private:
      twoDArrayHandling< dtPoint2 > correctConstV( 
        dtGmshFace const * const dtgf, twoDArrayHandling< dtPoint2 > pUV,
        int const & pos, int const & nSteps, 
        std::vector< double > lengths_i, double const & L_i
      ) const;
      static twoDArrayHandling< dtPoint2 > linearInterpolateU( 
        twoDArrayHandling< dtPoint2 > pUV, int const & vStart, int const & vEnd
      );
    private:
//      std::vector< float > _uCorrPos;
//      int _uCorrSteps;
      std::vector< float > _vCorrPos;
      int _vCorrSteps;
  };
}
#endif	/* dtMeshTransfiniteGFace_H */

