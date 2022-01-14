#ifndef dtMeshGFaceWithTransfiniteLayer_H
#define	dtMeshGFaceWithTransfiniteLayer_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh2DOperator.h"

class GFace;
class MVertex;

namespace dtOO {
  class dtGmshFace;
  class scaOneD;
  
  class dtMeshGFaceWithTransfiniteLayer : public dtMesh2DOperator {
    public:
      dt__class(dtMeshGFaceWithTransfiniteLayer, dtMeshOperator);     
      dtMeshGFaceWithTransfiniteLayer();
      dtMeshGFaceWithTransfiniteLayer(
        const dtMeshGFaceWithTransfiniteLayer& orig
      );
      virtual ~dtMeshGFaceWithTransfiniteLayer();
      virtual void init(
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
      int _direction;
      std::vector< int > _nLayers;
      int _nSmooth;
  };
}
#endif	/* dtMeshGFaceWithTransfiniteLayer_H */

