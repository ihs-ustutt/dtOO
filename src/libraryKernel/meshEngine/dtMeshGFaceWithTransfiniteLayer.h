#ifndef dtMeshGFaceWithTransfiniteLayer_H
#define	dtMeshGFaceWithTransfiniteLayer_H

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
        vectorHandling< constValue * > const * const cV,
        vectorHandling< analyticFunction * > const * const aF,
        vectorHandling< analyticGeometry * > const * const aG,
        vectorHandling< boundedVolume * > const * const bV,
        vectorHandling< dtMeshOperator * > const * const mO      
      );      
      void operator()( dtGmshFace * dtgr );    
//    private:
//      static void computeEdgeLoops(
//        const ::GFace *gf, 
//        std::vector< ::MVertex * > & all_mvertices, 
//        std::vector< int > & indices,
//        std::vector<MVertex*> & corners,
//        std::vector <MVertex *> & m_vertices                  
//      );
    private:
      int _direction;
      int _nLayers;
      int _nSmooth;
//      dt__pH(scaOneD) _alpha_2;
//      dt__pH(scaOneD) _beta_1;
//      dt__pH(scaOneD) _beta_2;
//      dt__pH(scaOneD) _beta_3;
  };
}
#endif	/* dtMeshGFaceWithTransfiniteLayer_H */

