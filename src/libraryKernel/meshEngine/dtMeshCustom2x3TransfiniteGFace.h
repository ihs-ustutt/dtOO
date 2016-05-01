#ifndef dtMeshCustom2x3TransfiniteGFace_H
#define	dtMeshCustom2x3TransfiniteGFace_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtMesh2DOperator.h"

class GFace;
class MVertex;

namespace dtOO {
  class dtGmshFace;
  class scaOneD;
  
  class dtMeshCustom2x3TransfiniteGFace : public dtMesh2DOperator {
    public:
      dt__class(dtMeshCustom2x3TransfiniteGFace, dtMeshOperator);     
      dtMeshCustom2x3TransfiniteGFace();
      dtMeshCustom2x3TransfiniteGFace(
        const dtMeshCustom2x3TransfiniteGFace& orig
      );
      virtual ~dtMeshCustom2x3TransfiniteGFace();
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
    private:
      static void computeEdgeLoops(
        const ::GFace *gf, 
        std::vector< ::MVertex * > & all_mvertices, 
        std::vector< int > & indices,
        std::vector<MVertex*> & corners,
        std::vector <MVertex *> & m_vertices                  
      );
    private:
      dt__pH(scaOneD) _alpha_1;
      dt__pH(scaOneD) _alpha_2;
      dt__pH(scaOneD) _beta_1;
      dt__pH(scaOneD) _beta_2;
      dt__pH(scaOneD) _beta_3;
  };
}
#endif	/* dtMeshCustom2x3TransfiniteGFace_H */

