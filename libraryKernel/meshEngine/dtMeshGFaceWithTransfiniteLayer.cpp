#include "dtMeshGFaceWithTransfiniteLayer.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include "dtMeshGFace.h"
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <gmsh/meshGFace.h>
#include <gmsh/MLine.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>

namespace dtOO {
  dtMeshGFaceWithTransfiniteLayer::dtMeshGFaceWithTransfiniteLayer() 
  : 
  dtMesh2DOperator() {
    
  }

  dtMeshGFaceWithTransfiniteLayer::dtMeshGFaceWithTransfiniteLayer(
    const dtMeshGFaceWithTransfiniteLayer& orig
  ) : dtMesh2DOperator(orig) {
    _direction = orig._direction;
    _nLayers = orig._nLayers;
  }

  dtMeshGFaceWithTransfiniteLayer::~dtMeshGFaceWithTransfiniteLayer() {
    
  }

  void dtMeshGFaceWithTransfiniteLayer::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO      
  ) {
    dtMesh2DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _direction = qtXmlBase::getAttributeIntMuParse("direction", element, cV);
    _nLayers = qtXmlBase::getAttributeIntMuParse("nLayers", element, cV);     
  }
      
  void dtMeshGFaceWithTransfiniteLayer::operator()( dtGmshFace * dtgf ) {

    dtgf->model()->setCurrentMeshEntity(dtgf);

    // destroy the mesh if it exists
    deMeshGFace()(dtgf);    
    
    Msg::Info(
      "Meshing surface %d ( dtMeshGFaceWithTransfiniteLayer )", dtgf->tag()
    );

    /*       0,H          L,H     
             +------------+
             |            |
             |            |
             |            |
             |            |
             +------------+
             0,0          L,0
    */
    //
    // order orientation and edge
    //
    std::vector< ::GEdge * > edges = progHelper::list2Vector( dtgf->edges() );
    std::vector< int > ori = progHelper::list2Vector( dtgf->orientations() );
    std::vector< std::pair< int, ::GEdge * > > ordered(4);
    if (_direction == 0) {
      ordered[0] = std::pair< int, ::GEdge * >(ori[0], edges[0]);
      ordered[1] = std::pair< int, ::GEdge * >(ori[1], edges[1]);
      ordered[2] = std::pair< int, ::GEdge * >(ori[2], edges[2]);
      ordered[3] = std::pair< int, ::GEdge * >(ori[3], edges[3]);
    }
    else if (_direction == 1) {
      ordered[0] = std::pair< int, ::GEdge * >(ori[1], edges[1]);
      ordered[1] = std::pair< int, ::GEdge * >(ori[2], edges[2]);
      ordered[2] = std::pair< int, ::GEdge * >(ori[3], edges[3]);
      ordered[3] = std::pair< int, ::GEdge * >(ori[0], edges[0]);
    }
    else dt__throwUnexpected(operator()());

    //
    // create edgeVertex array
    //
    /*       0,H    >(2)  L,H     
             +------------+
             |            |
             |            |
        (3) A|            |A (1)
             |            |
             +------------+
             0,0   >(0)   L,0
    */    
    twoDArrayHandling< ::MVertex * > edgeVertex(4,0);
    dt__forFromToIndex(0, 4, ii) {
      edgeVertex[ii].push_back( 
        ordered[ii].second->getBeginVertex()->mesh_vertices[0] 
      );
      dt__forAllRefAuto(ordered[ii].second->mesh_vertices, aVert) {
       edgeVertex[ii].push_back( aVert );
      }
      edgeVertex[ii].push_back( 
        ordered[ii].second->getEndVertex()->mesh_vertices[0] 
      );
      
      if ( ordered[ii].first != 1) progHelper::reverse( edgeVertex[ii] );
    }
    progHelper::reverse( edgeVertex[2] );
    progHelper::reverse( edgeVertex[3] );
    
    //
    // reparam edgeVertex array
    //
    twoDArrayHandling< dtPoint2 > edgeVertexUV(4, 0);    
    dt__forFromToIndex(0, 4, ii) {
      dt__forAllRefAuto( edgeVertex[ii], aVert) {
      SPoint2 param;
      reparamMeshVertexOnFace(aVert, dtgf, param);        
        edgeVertexUV[ii].push_back( dtPoint2(param[0], param[1]) );
      }
    }
    
    //
    // sheet 0
    //
    twoDArrayHandling< ::MVertex * > sheet0(edgeVertex[0].size(), _nLayers+1);
    dt__forAllIndex(edgeVertex[0], ii) sheet0[ii][0] = edgeVertex[0][ii];
    dt__forFromToIndex(1, _nLayers+1, layer) {
      sheet0[0                     ][layer] = edgeVertex[3][layer];
      sheet0[edgeVertex[0].size()-1][layer] = edgeVertex[1][layer];
      dt__forInnerIndex(sheet0.fixJ(layer), node) {
        dtPoint2 uv
        =
        edgeVertexUV[0][node]
        +
        (edgeVertexUV[3][layer] - edgeVertexUV[3][0]);
        
        ::GPoint pp = dtgf->point(uv.x(), uv.y());
        sheet0[node][layer]
        = 
        new ::MFaceVertex(pp.x(), pp.y(), pp.z(), dtgf, uv.x(), uv.y());
      }
    }

    //
    // sheet 1
    //
    progHelper::reverse( edgeVertex[1] );    
    progHelper::reverse( edgeVertex[2] );
    progHelper::reverse( edgeVertex[3] );    
    progHelper::reverse( edgeVertexUV[1] );
    progHelper::reverse( edgeVertexUV[2] );
    progHelper::reverse( edgeVertexUV[3] );      
    twoDArrayHandling< ::MVertex * > sheet1(edgeVertex[2].size(), _nLayers+1);
    dt__forAllIndex(edgeVertex[2], ii) sheet1[ii][0] = edgeVertex[2][ii];
    dt__forFromToIndex(1, _nLayers+1, layer) {
      sheet1[0                     ][layer] = edgeVertex[1][layer];
      sheet1[edgeVertex[2].size()-1][layer] = edgeVertex[3][layer];
      dt__forInnerIndex(sheet1.fixJ(layer), node) {
        dtPoint2 uv
        =
        edgeVertexUV[2][node]
        +
        (edgeVertexUV[3][layer] - edgeVertexUV[3][0]);
        
        ::GPoint pp = dtgf->point(uv.x(), uv.y());
        sheet1[node][layer] 
        = 
        new ::MFaceVertex(
          pp.x(), pp.y(), pp.z(), dtgf, uv.x(), uv.y()
        );
      }
    }
    progHelper::reverse( edgeVertex[1] );
    progHelper::reverse( edgeVertex[2] );
    progHelper::reverse( edgeVertex[3] );  
    progHelper::reverse( edgeVertexUV[1] );
    progHelper::reverse( edgeVertexUV[2] );
    progHelper::reverse( edgeVertexUV[3] );      
    
    //
    // create pseudo model, face and edge
    //
    dtGmshModel gm("myModel");
    ::GModel::setCurrent( &gm );
    dtGmshEdge * ge = new dtGmshEdge(&gm, 1);
    gm.add(ge);        
    dtGmshFace * gf = new dtGmshFace(&gm, 1);
    gm.add(gf);
    gf->addEdge( ge, 1 );
    ge->addFace( gf );
    
    //
    // add mesh vertices, mesh elements and underlying geometry
    //
    dt__forAllRefAuto(sheet0.fixJ(_nLayers), aVert) ge->addMeshVertex(aVert);
    dt__forFromToIndex(_nLayers+1, edgeVertex[1].size()-_nLayers-1, ii) {
      ge->addMeshVertex( edgeVertex[1][ii] );
    }
    std::vector< ::MVertex * > tmpSheet = sheet1.fixJ(_nLayers);
    dt__forAllRefAuto(tmpSheet, aVert) ge->addMeshVertex(aVert);
    progHelper::reverse( edgeVertex[3] );
    dt__forFromToIndex(_nLayers+1, edgeVertex[3].size()-_nLayers-1, ii) {
      ge->addMeshVertex( edgeVertex[3][ii] );
    }
    progHelper::reverse( edgeVertex[3] );
    
    dt__forFromToIndex(1, ge->getNumMeshVertices(), ii) {
      ge->addLine( 
        new ::MLine(ge->getMeshVertex(ii-1), ge->getMeshVertex(ii)) 
      );
    }
    ge->addLine( 
      new ::MLine(
        ge->getMeshVertex(ge->getNumMeshVertices()-1), ge->getMeshVertex(0)
      ) 
    );
    ge->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
    gf->setMap2dTo3d( dtgf->getMap2dTo3d() );
    
    //
    // perform meshing
    //
//    gm.writeMSH(
//      dtgf->getMap2dTo3d()->getLabel()+"dtMeshGFaceWithTransfiniteLayer_0.msh", 
//      2.2, false, true
//    );    
    dtMeshGFace()( gf );
//    gm.writeMSH(
//      dtgf->getMap2dTo3d()->getLabel()+"dtMeshGFaceWithTransfiniteLayer_1.msh", 
//      2.2, false, true
//    );
    
    //
    // change back to old model
    //
    ::GModel::setCurrent( dtgf->model() );
    
    //
    // add transfinite layer quadrangles
    //
    dt__forFromToIndex(0, _nLayers, layer) {
      dt__forFromToIndex(0, sheet0.size()-1, node) {
        dtgf->addElement(
          new MQuadrangle(
            sheet0[node][layer], 
            sheet0[node+1][layer], 
            sheet0[node+1][layer+1], 
            sheet0[node][layer+1]
          )  
        );
      }
    }
    dt__forFromToIndex(0, _nLayers, layer) {
      dt__forFromToIndex(0, sheet1.size()-1, node) {
        dtgf->addElement(
          new MQuadrangle(
            sheet1[node][layer], 
            sheet1[node+1][layer], 
            sheet1[node+1][layer+1], 
            sheet1[node][layer+1]
          )
        );
      }
    }
    //
    // add transfinite layer vertices
    //
    dt__forFromToIndex(0, _nLayers+1, layer) {
      dt__forFromToIndex(0, sheet0.size(), node) {
        if ( sheet0[node][layer]->onWhat() == dtgf ) {
          dtgf->addMeshVertex(sheet0[node][layer]);
        }
      }
    }
    dt__forFromToIndex(0, _nLayers+1, layer) {
      dt__forFromToIndex(0, sheet1.size(), node) {
        if ( sheet1[node][layer]->onWhat() == dtgf ) {
          dtgf->addMeshVertex(sheet1[node][layer]);
        }
      }
    }    
    
    //
    // add unstructured mesh vertices
    //
    dt__forFromToIndex(0, gf->getNumMeshVertices(), ii) {
      gf->getMeshVertex(ii)->setEntity( dtgf );
      dtgf->addMeshVertex( gf->getMeshVertex(ii) );
      
      dtgf->model()->setMaxVertexNumber(
        dtgf->model()->getMaxVertexNumber() + 1
      );
      gf->getMeshVertex(ii)->setNum( 
        dtgf->model()->getMaxVertexNumber() 
      );
    }

    //
    // add unstructured mesh elements
    //
    dt__forFromToIndex(0, gf->getNumMeshElements(), ii) {
      dtgf->addElement( gf->getMeshElement(ii) );
      gf->getMeshElement(ii)->reverse();
    }
    
    //
    // free memory of pseudo GEntities
    ge->mesh_vertices.clear();
    gf->mesh_vertices.clear();
    gf->triangles.clear();    
    
    //
    // set done
    //
    dtgf->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
    
//    dtgf->refDtGmshModel().writeMSH(
//      dtgf->getMap2dTo3d()->getLabel()+"dtMeshGFaceWithTransfiniteLayer_2.msh", 
//      2.2, false, true
//    );    
  }
}