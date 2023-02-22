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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO      
      );      
      void operator()( dtGmshFace * dtgr );    
    private:
      dtInt _direction;
      std::vector< dtInt > _nLayers;
      dtInt _nSmooth;
  };
}
#endif	/* dtMeshGFaceWithTransfiniteLayer_H */

