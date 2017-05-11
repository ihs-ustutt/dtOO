#include "dtOptimizeMeshGRegion.h"

#include <logMe/logMe.h>
#include <gmsh/MVertex.h>
#include <gmsh/meshGRegion.h>
#include "dtGmshFace.h"
#include "dtGmshRegion.h"
#include "dtGmshModel.h"
//#include <gmsh/Context.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>

namespace dtOO {
  dtOptimizeMeshGRegion::dtOptimizeMeshGRegion() : dtMesh3DOperator() {
  }

  dtOptimizeMeshGRegion::~dtOptimizeMeshGRegion() {
  }
  
  dtOptimizeMeshGRegion::dtOptimizeMeshGRegion(
    const dtOptimizeMeshGRegion& orig
  ) : dtMesh3DOperator(orig) {
    
  }
  
  void dtOptimizeMeshGRegion::operator()( dtGmshRegion * dtgr) {
    Msg::Info("dtOptimizeMeshGRegion()() volume %d", dtgr->tag());
    
    //
    // create cloneRegion and cloneFace
    //
    dtGmshRegion cloneRegion(
      dtgr->model(), dtgr->refDtGmshModel().getMaxRegionTag()+1
    );
    dtGmshFace cloneFace( 
      dtgr->model(), dtgr->refDtGmshModel().getMaxFaceTag()+1
    );
    dtGmshFace cloneFaceApex( 
      dtgr->model(), dtgr->refDtGmshModel().getMaxFaceTag()+1
    );
    
    //
    // add region to model
    //
    cloneRegion.addFace( &cloneFace, 1 );
    cloneRegion.addFace( &cloneFaceApex, 1 );
    dtgr->model()->add( &cloneFace );
    dtgr->model()->add( &cloneFaceApex );
    dtgr->model()->add( &cloneRegion );    

    //
    // add mesh entities to cloneFace
    //
    dt__forAllRefAuto( dtgr->vertices(), anEnt ) {
      dt__forAllRefAuto( anEnt->mesh_vertices, aVert ) {
        cloneFace.addMeshVertex(aVert);
      }
    }    
    dt__forAllRefAuto( dtgr->edges(), anEnt ) {
      dt__forAllRefAuto( anEnt->mesh_vertices, aVert ) {
        cloneFace.addMeshVertex(aVert);
      }
    }
    dt__forAllRefAuto( dtgr->faces(), aFace ) {
      //
      // mesh vertices
      //
      dt__forAllRefAuto( aFace->mesh_vertices, aVert ) {
        cloneFace.addMeshVertex(aVert);
      }
      //
      // mesh triangles
      //
      if (aFace->triangles.size()) {
        dt__forAllRefAuto(aFace->triangles, aTri) {
          cloneFace.addElement( (::MElement*) aTri );
        }
      }
    }
    
    //
    // tetrahedra
    //
    dt__forAllRefAuto(dtgr->tetrahedra, aTet) {
      cloneRegion.addElement((::MElement*)aTet);
    }    
    //
    // pyramids
    //
    dt__forAllRefAuto(dtgr->pyramids, aPyr) {
      dt__throwIf(
        aPyr->getVertex(0)->onWhat()->dim() > 2
        &&
        aPyr->getVertex(1)->onWhat()->dim() > 2
        &&
        aPyr->getVertex(2)->onWhat()->dim() > 2
        &&
        aPyr->getVertex(3)->onWhat()->dim() > 2,
        operator()()        
      );
      aPyr->getVertex(4)->setEntity(&cloneFaceApex);
      cloneFaceApex.addElement( 
        new ::MTriangle(
          aPyr->getVertex(0), aPyr->getVertex(1), aPyr->getVertex(4)
        ) 
      );
      cloneFaceApex.addElement( 
        new ::MTriangle(
          aPyr->getVertex(1), aPyr->getVertex(2), aPyr->getVertex(4)
        ) 
      );
      cloneFaceApex.addElement( 
        new ::MTriangle(
          aPyr->getVertex(2), aPyr->getVertex(3), aPyr->getVertex(4)
        ) 
      );
      cloneFaceApex.addElement( 
        new ::MTriangle(
          aPyr->getVertex(3), aPyr->getVertex(0), aPyr->getVertex(4)
        ) 
      );      
    }
    //
    // add entities to cloneRegion
    //
    dt__forAllRefAuto( dtgr->mesh_vertices, aVert ) {
      if ( aVert->onWhat() != &cloneFaceApex) {      
        cloneRegion.addMeshVertex(aVert);
        aVert->setEntity(&cloneRegion);
      }
    }    
    
    //
    // do optimization
    //
    ::optimizeMeshGRegionNetgen()(&cloneRegion);
    ::optimizeMeshGRegionGmsh()(&cloneRegion);
    
    //
    // clean and reset dtgr mesh
    //
    std::vector< ::MVertex * > newVert;
    dt__forAllRefAuto(dtgr->mesh_vertices, aVert) {
      if ( aVert->onWhat() == &cloneFaceApex) {
        aVert->setEntity( dtgr );
        newVert.push_back( aVert );
      }
    }
    dtgr->mesh_vertices.clear();
    dt__forAllRefAuto(newVert, aVert) dtgr->addMeshVertex(aVert);
    dtgr->tetrahedra.clear();
    
    //
    // transfer mesh vertices and mesh elements of cloneFace, 
    // cloneFaceApex and cloneRegion back to dtgr
    //
    dt__forAllRefAuto(cloneRegion.mesh_vertices, aVert) {
      dtgr->addMeshVertex( aVert );
      aVert->setEntity( dtgr );
    }
    dt__forAllRefAuto(cloneRegion.tetrahedra, aTet) {
      dtgr->addElement( (::MElement*) aTet );
    }
    dt__forAllRefAuto(cloneFaceApex.triangles, aTri) delete aTri;    

    
    //
    // delete GEntities
    //    
    cloneFace.mesh_vertices.clear();
    cloneFace.triangles.clear();    
    cloneRegion.mesh_vertices.clear();
    cloneRegion.tetrahedra.clear();
    cloneFaceApex.mesh_vertices.clear();
    cloneFaceApex.triangles.clear();
    
    
    dtgr->model()->remove(&cloneRegion);
    dtgr->model()->remove(&cloneFace);    
    dtgr->model()->remove(&cloneFaceApex);
  }
}
