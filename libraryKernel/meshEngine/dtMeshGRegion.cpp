#include "dtMeshGRegion.h"

#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include "dtGmshFace.h"
#include <gmsh/GFace.h>
#include <gmsh/GRegion.h>
#include <gmsh/meshGRegion.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MPyramid.h>

namespace dtOO {
  dtMeshGRegion::dtMeshGRegion() {
  }

  dtMeshGRegion::dtMeshGRegion(const dtMeshGRegion& orig) {
  }

  dtMeshGRegion::~dtMeshGRegion() {
  }

  void dtMeshGRegion::operator()( dtGmshRegion * dtgr) {
    std::list< dtGmshFace * > faces 
    = 
    dtGmshModel::cast2DtGmshFace( dtgr->faces() );
    
    //
    // get number of quad faces
    //
    int quads = 0;
    dt__forAllIter(std::list< dtGmshFace * >, faces, it) {
      if ( (*it)->quadrangles.size() ) {
        quads++;
      }
    }
    
    if ( (quads>0) && (quads != faces.size()) ) {
      dt__info(
        operator(), 
        << "Bounding faces contain quadrangles." << std::endl
        << quads << " faces meshed with quads found." << std::endl
        << "Performing createPyramids()."
      );      
      createPyramids(dtgr);
    }
    else {
      dt__info(
        operator(), 
        << "Bounding faces contain no quadrangles." << std::endl
        << "Performing normal meshing."
      );      
      
      std::vector< ::GRegion * > delauny;
      ::meshGRegion mr( delauny );
      mr(dtgr);    
      MeshDelaunayVolume(delauny);
      dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;
    }
  }
  
  void dtMeshGRegion::createPyramids( dtGmshRegion * dtgr ) {
    std::list< dtGmshFace * > faces 
    = 
    dtGmshModel::cast2DtGmshFace( dtgr->faces() );
    
    std::map< dtGmshFace *, dtGmshFace * > pseudo_org;
    std::vector< ::MPyramid * > pyramids;
    std::vector< ::MVertex * > vertices;    
    
    dt__forAllIter(std::list< dtGmshFace * >, faces, it) {
      dtGmshFace * gf = *it;
      
      //
      // modify only quadrangle surfaces
      //
      if (gf->quadrangles.size() == 0) continue;

      dt__info(
        createPyramids(), 
        << "Face " << gf->tag() << " contains quadrangles." << std::endl
        << "Remove face temporarily."
      );
      

      //
      // create new pseudo face
      //
      std::list< int > ori = gf->edgeOrientations();
      dtGmshFace * pseudo 
      =  
      new dtGmshFace(
        gf->model(), 
        dtgr->refDtGmshModel().getMaxFaceTag()+1, 
        gf->edges(), progHelper::list2Vector(ori)
      );
      gf->model()->add(pseudo);
      
      //
      // store in mapping
      //
      pseudo_org[gf] = pseudo;
      
      //
      // replace face with pseudo face
      //
      dtgr->replaceFace(gf, pseudo);      
      
      //
      // create pyramids
      //
      std::vector< ::MQuadrangle * > const & quads = gf->quadrangles;
      dt__forAllConstIter(std::vector< ::MQuadrangle * >, quads, it) {
        //
        // calculate barycenter, inner radius and normal
        // 
        SPoint3 bb = (*it)->barycenter();
        SVector3 nn = (*it)->getFace(0).normal();
        double radius = (*it)->getInnerRadius();

        //
        // create new mesh vertex
        //
        vertices.push_back( 
          new ::MVertex(
            bb.x() - radius*nn.x() , bb.y() - radius*nn.y(), bb.z() - radius*nn.z(),
            pseudo
          )
        );
        pseudo->addMeshVertex(vertices.back());
        
        //
        // create new pseudo mesh elements (truangles)
        //
        pseudo->addTriangle(
          new ::MTriangle( (*it)->getVertex(0), (*it)->getVertex(1), vertices.back() )         
        );
        pseudo->addTriangle(
          new ::MTriangle( (*it)->getVertex(1), (*it)->getVertex(2), vertices.back() )         
        );      
        pseudo->addTriangle(
          new ::MTriangle( (*it)->getVertex(2), (*it)->getVertex(3), vertices.back() )         
        );      
        pseudo->addTriangle(
          new ::MTriangle( (*it)->getVertex(3), (*it)->getVertex(0), vertices.back() )         
        );      
       
        //
        // copy old face vertices
        //
        dt__forFromToIndex(0, 3, ii) {
          if ( (*it)->getVertex(ii)->onWhat()->dim()<2) continue;
          pseudo->addMeshVertex( (*it)->getVertex(ii) );        
        }
        
        pyramids.push_back(
          new ::MPyramid(
            (*it)->getVertex(0), 
            (*it)->getVertex(1), 
            (*it)->getVertex(2),
            (*it)->getVertex(3),
            vertices.back()
          )
        );
      }
      pseudo->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
    }
    
    dt__debug(
      createPyramids(), 
      << "Region: " << std::endl
      << dtgr->dumpToString()
    );

    dtgr->model()->writeMSH("dtMeshGRegion.msh");
    
    //
    // call meshing again
    //
    dtMeshGRegion()(dtgr);
    
    dt__forAllIter(std::vector< ::MVertex * >, vertices, it) {
      (*it)->setEntity(dtgr);
      dtgr->addMeshVertex(*it);
    }
    dt__forAllIter(std::vector< ::MPyramid * >, pyramids, it) {
      dtgr->addPyramid( (*it) );
    }
    
    for (auto it : pseudo_org) {
      it.second->mesh_vertices.clear();
      it.second->deleteMesh();
      dtgr->replaceFace(it.second, it.first);        
      dtgr->model()->remove(it.second);
    }
    
  }
}
