#include "dtMeshGFaceWithTransfiniteLayer.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include "dtGmshEdge.h"
#include "dtGmshFace.h"
#include "dtGmshModel.h"
#include "dtMeshGFace.h"
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <gmsh/meshGFace.h>
#include "dtMeshTransfiniteGFace.h"
#include <gmsh/MLine.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MQuadrangle.h>
#include <interfaceHeaven/threadSafe.h>
#include <progHelper.h>

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
    _nSmooth = orig._nSmooth;
  }

  dtMeshGFaceWithTransfiniteLayer::~dtMeshGFaceWithTransfiniteLayer() {
    
  }

  void dtMeshGFaceWithTransfiniteLayer::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO      
  ) {
    dtMesh2DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _direction = qtXmlBase::getAttributeIntMuParse("direction", element, cV);
    _nLayers = std::vector< int >(2,0);
    if ( qtXmlPrimitive::isAttributeVector("nLayers", element) ) {
      _nLayers
      = 
      qtXmlBase::getAttributeIntVectorMuParse("nLayers", element, cV, aF);
    }
    else {
      _nLayers[0]
      =       
      qtXmlBase::getAttributeIntMuParse("nLayers", element, cV),
      _nLayers[1] = _nLayers[0];
    }
    _nSmooth = 0;
    if ( qtXmlBase::hasAttribute("nSmooth", element) ) {
      _nSmooth = qtXmlBase::getAttributeIntMuParse("nSmooth", element, cV);       
    }
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
    std::vector< ::GEdge * > edges = dtgf->edges();
    std::vector< int > ori = dtgf->orientations();
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
    twoDArrayHandling< std::pair< ::MVertex *, dtPoint2 > > edgeVertex(4,0);
    dt__forFromToIndex(0, 4, ii) {
      //
      // allocate
      //
      edgeVertex[ii].resize( ordered[ii].second->mesh_vertices.size() + 2 );
      
      //
      // first vertex
      //
      ::MVertex * mv = ordered[ii].second->getBeginVertex()->mesh_vertices[0];
      SPoint2 param;
      reparamMeshVertexOnFace(mv, dtgf, param);        
      edgeVertex[ii][0] = std::make_pair( mv, dtPoint2(param[0], param[1]) );

      //
      // inner vertex
      //      
      #pragma omp parallel 
      {
        //
        // make threadSafe
        //
        threadSafe< dt__pH(map2dTo3d) > m2d;
        m2d().reset( dtgf->getMap2dTo3d()->clone() );

        #pragma omp for        
        dt__forAllIndex(ordered[ii].second->mesh_vertices, jj) {
          ::MVertex * aVert = ordered[ii].second->mesh_vertices[jj];
          edgeVertex[ii][jj+1] 
          = 
          std::make_pair( 
            aVert, 
            m2d()->reparamOnFace( dtGmshModel::extractPosition(aVert) )
          );        
        }
      }
      
      //
      // last vertex
      //
      mv = ordered[ii].second->getEndVertex()->mesh_vertices[0] ;
      reparamMeshVertexOnFace(mv, dtgf, param);        
      edgeVertex[ii].back()
      =
      std::make_pair( mv, dtPoint2(param[0], param[1]) );      
      
      //
      // correct orientation
      //
      if ( ordered[ii].first != 1) progHelper::reverse( edgeVertex[ii] );
    }
    progHelper::reverse( edgeVertex[2] );
    progHelper::reverse( edgeVertex[3] );
    
    
    //
    // sheet 0
    //
    twoDArrayHandling< ::MVertex * > sheet0(
      edgeVertex[0].size(), _nLayers[0]+1
    );
    dt__forAllIndex(edgeVertex[0], ii) sheet0[ii][0] = edgeVertex[0][ii].first;
    dt__forFromToIndex(1, _nLayers[0]+1, layer) {
      dt__info( operator()(), << "Create layer " << layer << " on sheet0." );
      sheet0[0                     ][layer] = edgeVertex[3][layer].first;
      sheet0[edgeVertex[0].size()-1][layer] = edgeVertex[1][layer].first;

      //
      // determine spacing
      //
      dtVector3 dist0(
        edgeVertex[3][layer].first->x() - edgeVertex[3][0].first->x(),
        edgeVertex[3][layer].first->y() - edgeVertex[3][0].first->y(),
        edgeVertex[3][layer].first->z() - edgeVertex[3][0].first->z()
      );
      dtVector3 dist1(
        edgeVertex[1][layer].first->x() - edgeVertex[1][0].first->x(),
        edgeVertex[1][layer].first->y() - edgeVertex[1][0].first->y(),
        edgeVertex[1][layer].first->z() - edgeVertex[1][0].first->z()
      );
      float distInc 
      = 
      ( 
        dtLinearAlgebra::length( dist1 )
        -
        dtLinearAlgebra::length( dist0 )
      )
      /
      ( sheet0.fixJ(layer).size() - 1);
      
      //
      // calculate normals
      //
      std::vector< dtVector3 > NN(sheet0.fixJ(layer).size());
      NN[ 0 ] 
      = 
      dtLinearAlgebra::normalize( dist0 );
      NN[ sheet0.fixJ(layer).size() - 1 ] 
      = 
      dtLinearAlgebra::normalize( dist1 );
        
      dt__forInnerIndex(sheet0.fixJ(layer), node) {
        //
        // get normal to boundary
        //
        NN[ node ]
        =
        dtLinearAlgebra::normalize(
          dtLinearAlgebra::crossProduct(
            dtVector3(
              edgeVertex[0][node+1].first->x() 
              - 
              edgeVertex[0][node-1].first->x(),
              edgeVertex[0][node+1].first->y() 
              - 
              edgeVertex[0][node-1].first->y(),
              edgeVertex[0][node+1].first->z() 
              - 
              edgeVertex[0][node-1].first->z()
            ),
            dtgf->normal( edgeVertex[0][node].second )
          )
        );
        if (dtLinearAlgebra::dotProduct(NN[ node ], NN[ node - 1 ]) < 0.) {
          NN[ node ] = -1. * NN[ node ];          
        }
      }
      if (
        dtLinearAlgebra::dotProduct(
          NN[ sheet0.fixJ(layer).size() - 1 ], 
          NN[ sheet0.fixJ(layer).size() - 2 ]
        ) < 0.
      ) {
        NN[ sheet0.fixJ(layer).size() - 1 ] 
        = 
        -1. * NN[ sheet0.fixJ(layer).size() - 1 ];          
      }
      
      //
      // smooth normals
      //
      dt__forFromToIndex(0, _nSmooth, thisSmooth) {
        dt__forInnerIndex(sheet0.fixJ(layer), node) {
          NN[ node ]
          =
          dtLinearAlgebra::normalize( NN[ node - 1] + NN[ node + 1] );
        }
      }
      
      dt__forInnerIndex(sheet0.fixJ(layer), node) {
        dtVector3 nn = NN[ node ];
        //
        // correct orientation
        //
        dtVector3 checkDist(
          sheet0[node-1][layer]->x() - sheet0[node-1][layer-1]->x(),
          sheet0[node-1][layer]->y() - sheet0[node-1][layer-1]->y(),
          sheet0[node-1][layer]->z() - sheet0[node-1][layer-1]->z()
        );
        if (dtLinearAlgebra::dotProduct(nn, checkDist) < 0.) nn = -1. * nn;        

        //
        // transform normal to parameter coordinates
        //
        dtVector2 nnUV
        =
        dtLinearAlgebra::toDtVector2(
          dtLinearAlgebra::solveMatrix(
            dtgf->getMap2dTo3d()->jacobi( edgeVertex[0][node].second )
            ,
            dtLinearAlgebra::createMatrixVector(
              ( dtLinearAlgebra::length( dist0 ) + distInc * node ) * nn
            )
          )
        );

        //
        // create new point
        //
        dtPoint2 uv = edgeVertex[0][node].second + nnUV;
        ::GPoint pp = dtgf->point(uv.x(), uv.y());
        sheet0[node][layer]
        = 
        new ::MFaceVertex(pp.x(), pp.y(), pp.z(), dtgf, uv.x(), uv.y());
      }
    }
//    }

    //
    // sheet 1
    //
    progHelper::reverse( edgeVertex[1] );    
    progHelper::reverse( edgeVertex[2] );
    progHelper::reverse( edgeVertex[3] );    
    twoDArrayHandling< ::MVertex * > sheet1(
      edgeVertex[2].size(), _nLayers[1]+1
    );
    dt__forAllIndex(edgeVertex[2], ii) sheet1[ii][0] = edgeVertex[2][ii].first;
    dt__forFromToIndex(1, _nLayers[1]+1, layer) {
      dt__info( operator()(), << "Create layer " << layer << " on sheet1." );
      sheet1[0                     ][layer] = edgeVertex[1][layer].first;
      sheet1[edgeVertex[2].size()-1][layer] = edgeVertex[3][layer].first;

      //
      // determine spacing
      //
      dtVector3 dist0(
        edgeVertex[1][layer].first->x() - edgeVertex[1][0].first->x(),
        edgeVertex[1][layer].first->y() - edgeVertex[1][0].first->y(),
        edgeVertex[1][layer].first->z() - edgeVertex[1][0].first->z()
      );
      dtVector3 dist1(
        edgeVertex[3][layer].first->x() - edgeVertex[3][0].first->x(),
        edgeVertex[3][layer].first->y() - edgeVertex[3][0].first->y(),
        edgeVertex[3][layer].first->z() - edgeVertex[3][0].first->z()
      );
      float distInc 
      = 
      ( 
        dtLinearAlgebra::length( dist1 )
        -
        dtLinearAlgebra::length( dist0 )
      )
      /
      ( sheet1.fixJ(layer).size() - 1);
      
      std::vector< dtVector3 > NN( sheet1.fixJ(layer).size() );
      NN[ 0 ] = -1. * dtLinearAlgebra::normalize( dist0 );      
      NN[ sheet1.fixJ(layer).size() - 1 ] 
      = 
      -1. * dtLinearAlgebra::normalize( dist1 );
      
      dt__forInnerIndex(sheet1.fixJ(layer), node) {
        //
        // get normal to boundary
        //        
        NN[ node ]
        =
        dtLinearAlgebra::normalize(
          dtLinearAlgebra::crossProduct(
            dtVector3(
              edgeVertex[2][node+1].first->x() - edgeVertex[2][node-1].first->x(),
              edgeVertex[2][node+1].first->y() - edgeVertex[2][node-1].first->y(),
              edgeVertex[2][node+1].first->z() - edgeVertex[2][node-1].first->z()
            ),
            dtgf->normal( edgeVertex[2][node].second )
          )
        );
      }
      if (
        dtLinearAlgebra::dotProduct(
          NN[ sheet1.fixJ(layer).size() - 1 ], 
          NN[ sheet1.fixJ(layer).size() - 2 ]
        ) < 0.
      ) {
        NN[ sheet1.fixJ(layer).size() - 1 ] 
        = 
        -1. * NN[ sheet1.fixJ(layer).size() - 1 ];          
      }
      
      dt__forFromToIndex(0, _nSmooth, thisSmooth) {
        dt__forInnerIndex(sheet1.fixJ(layer), node) {
          NN[ node ]
          =
          dtLinearAlgebra::normalize( NN[ node - 1] + NN[ node + 1] );
        }
      }
      
      dt__forInnerIndex(sheet1.fixJ(layer), node) {
        dtVector3 nn = NN[ node ];        
        //
        // correct orientation
        //        
        dtVector3 checkDist(
          sheet1[node-1][layer]->x() - sheet1[node-1][layer-1]->x(),
          sheet1[node-1][layer]->y() - sheet1[node-1][layer-1]->y(),
          sheet1[node-1][layer]->z() - sheet1[node-1][layer-1]->z()
        );
        if (dtLinearAlgebra::dotProduct(nn, checkDist) < 0.) nn = -1. * nn;

        //
        // transform normal to parameter coordinates
        //        
        dtVector2 nnUV
        =
        dtLinearAlgebra::toDtVector2(
          dtLinearAlgebra::solveMatrix(
            dtgf->getMap2dTo3d()->jacobi( edgeVertex[2][node].second )
            ,
            dtLinearAlgebra::createMatrixVector(
              ( dtLinearAlgebra::length( dist0 ) + distInc * node ) * nn
            )
          )
        );

        //
        // create new point
        //        
        dtPoint2 uv = edgeVertex[2][node].second + nnUV;        
        ::GPoint pp = dtgf->point(uv.x(), uv.y());
        sheet1[node][layer] 
        = 
        new ::MFaceVertex(
          pp.x(), pp.y(), pp.z(), dtgf, uv.x(), uv.y()
        );
      }
    }
//    }
    progHelper::reverse( edgeVertex[1] );
    progHelper::reverse( edgeVertex[2] );
    progHelper::reverse( edgeVertex[3] );  
    
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
    dt__forAllRefAuto(sheet0.fixJ(_nLayers[0]), aVert) {
      ge->addMeshVertex(aVert);
    }
    dt__forFromToIndex(
      _nLayers[0]+1, edgeVertex[1].size()-_nLayers[1]-1, ii
    ) {
      ge->addMeshVertex( edgeVertex[1][ii].first );
    }
    std::vector< ::MVertex * > tmpSheet = sheet1.fixJ(_nLayers[1]);
    dt__forAllRefAuto(tmpSheet, aVert) ge->addMeshVertex(aVert);
    progHelper::reverse( edgeVertex[3] );
    dt__forFromToIndex(
      _nLayers[1]+1, edgeVertex[3].size()-_nLayers[0]-1, ii
    ) {
      ge->addMeshVertex( edgeVertex[3][ii].first );
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
    
    if ( optionHandling::debugTrue() ) {
      gm.writeMSH(
        dtgf->getMap2dTo3d()->getLabel()+"dtMeshGFaceWithTransfiniteLayer_0.msh", 
        2.2, false, true
      );    
    }

    //
    // perform meshing
    //    
    dtMeshGFace()( gf );

    if ( optionHandling::debugTrue() ) {    
      gm.writeMSH(
        dtgf->getMap2dTo3d()->getLabel()+"dtMeshGFaceWithTransfiniteLayer_1.msh", 
        2.2, false, true
      );
    }
    
    //
    // change back to old model
    //
    ::GModel::setCurrent( dtgf->model() );
    
    //
    // add transfinite layer quadrangles
    //
    dt__forFromToIndex(0, _nLayers[0], layer) {
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
    dt__forFromToIndex(0, _nLayers[1], layer) {
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
    dt__forFromToIndex(0, _nLayers[0]+1, layer) {
      dt__forFromToIndex(0, sheet0.size(), node) {
        if ( sheet0[node][layer]->onWhat() == dtgf ) {
          dtgf->addMeshVertex(sheet0[node][layer]);
        }
      }
    }
    dt__forFromToIndex(0, _nLayers[1]+1, layer) {
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
    
    if ( optionHandling::debugTrue() ) {
      dtgf->refDtGmshModel().writeMSH(
        dtgf->getMap2dTo3d()->getLabel()
        +
        "dtMeshGFaceWithTransfiniteLayer_2.msh", 
        2.2, false, true
      );    
    }
  }
}