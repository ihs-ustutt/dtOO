#ifndef DTOPTIMIZEMESHGREGION_H
#define	DTOPTIMIZEMESHGREGION_H

#include <logMe/dtMacros.h>
#include "dtMesh3DOperator.h"



//class Ng_Mesh;
//class Ng_Result;
class GRegion;
void meshNormalsPointOutOfTheRegion( ::GRegion *gr );

//Ng_Mesh *buildNetgenStructure(
//  GRegion *gr, bool importVolumeMesh, std::vector<MVertex*> &numberedV
//);
//Ng_Result Ng_OptimizeVolumeMesh( Ng_Mesh *mesh, double maxh );
//void TransferVolumeMesh(
//  GRegion *gr, Ng_Mesh *ngmesh, std::vector<MVertex*> &numberedV
//);
//void Ng_DeleteMesh (Ng_Mesh * mesh);
//void Ng_Exit();

namespace dtOO {
  class dtGmshRegion;
  
  class dtOptimizeMeshGRegion : public dtMesh3DOperator {
  public:
    dt__classOnlyName(dtOptimizeMeshGRegion);     
    dtOptimizeMeshGRegion();
    dtOptimizeMeshGRegion(const dtOptimizeMeshGRegion& orig);    
    virtual ~dtOptimizeMeshGRegion();
    void operator()( dtGmshRegion * dtgr );    
  private:

  };
}
#endif	/* DTOPTIMIZEMESHGREGION_H */
