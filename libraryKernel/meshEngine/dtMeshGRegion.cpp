#include "dtMeshGRegion.h"

#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include "dtGmshFace.h"
#include "xmlHeaven/qtXmlBase.h"
#include <gmsh/meshGRegion.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MPyramid.h>

namespace dtOO {
  dtMeshGRegion::dtMeshGRegion() : dtMesh3DOperator() {
    _pyramidHeightScale = .25;
  }

  dtMeshGRegion::dtMeshGRegion(
    const dtMeshGRegion& orig
  ) : dtMesh3DOperator(orig) {
    _pyramidHeightScale = orig._pyramidHeightScale;
  }

  dtMeshGRegion::~dtMeshGRegion() {
  }
  
  void dtMeshGRegion::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _pyramidHeightScale 
    = 
    qtXmlBase::getAttributeFloatMuParse("pyramidHeightScale", element, cV, aF);
  }

  void dtMeshGRegion::operator()( dtGmshRegion * dtgr) {
    std::list< dtGmshFace * > faces 
    = 
    dtGmshModel::cast2DtGmshFace( dtgr->faces() );
    
    //
    // get number of quad faces
    //
    int quadFaces = 0;
    int quads = 0;
    int elems = 0;
    dt__forAllIter(std::list< dtGmshFace * >, faces, it) {
      quads = quads + (*it)->quadrangles.size();
      elems = elems + (*it)->getNumMeshElements();
      if ( (*it)->quadrangles.size() ) {
        quadFaces++;
      }
    }
    
    if ( (quads>0) && (quads != elems) ) {
      dt__info(
        operator(), 
        << "Bounding faces contain " << quads << " quadrangles and "
        << elems << " elements in general." << std::endl
        << "=> " << (float) quads / (float) elems 
        << " % quadrangles" << std::endl
        << quadFaces << " faces meshed with quads found." << std::endl
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
      // copy triangles
      //
      std::vector< ::MTriangle * > const & tri = gf->triangles;
      dt__forAllConstIter(std::vector< ::MTriangle * >, tri, it) {        
        //
        // create new pseudo mesh elements (truangles)
        //
        pseudo->addTriangle(
          new ::MTriangle( 
            (*it)->getVertex(0), (*it)->getVertex(1), (*it)->getVertex(2)
          )
        );
       
        //
        // copy old face vertices
        //
        dt__forFromToIndex(0, 2, ii) {
          if ( (*it)->getVertex(ii)->onWhat()->dim()<2) continue;
          pseudo->addMeshVertex( (*it)->getVertex(ii) );        
        }        
      }
      
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
        double radius = _pyramidHeightScale * (*it)->getInnerRadius();

        //
        // create new mesh vertex
        //
        vertices.push_back( 
          new ::MVertex(
            bb.x() - radius*nn.x() , 
            bb.y() - radius*nn.y(), 
            bb.z() - radius*nn.z(),
            pseudo
          )
        );
        pseudo->addMeshVertex(vertices.back());
        
        //
        // create new pseudo mesh elements (truangles)
        //
        pseudo->addTriangle(
          new ::MTriangle( 
            (*it)->getVertex(0), (*it)->getVertex(1), vertices.back() 
          )
        );
        pseudo->addTriangle(
          new ::MTriangle( 
            (*it)->getVertex(1), (*it)->getVertex(2), vertices.back() 
          )
        );      
        pseudo->addTriangle(
          new ::MTriangle( 
            (*it)->getVertex(2), (*it)->getVertex(3), vertices.back() 
          )
        );      
        pseudo->addTriangle(
          new ::MTriangle( 
            (*it)->getVertex(3), (*it)->getVertex(0), vertices.back() 
          )
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
    this->operator()(dtgr);
    
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

