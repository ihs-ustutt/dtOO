#include "dtMeshGRegion.h"

#include "dtGmshRegion.h"
#include "dtGmshModel.h"
#include "dtGmshFace.h"
#include <xmlHeaven/qtXmlBase.h>
#include <gmsh/meshGRegion.h>
#include <gmsh/MQuadrangle.h>
#include <gmsh/MTriangle.h>
#include <gmsh/MTetrahedron.h>
#include <gmsh/MPyramid.h>
#include <gmsh/MElementOctree.h>
#include "qShapeMetric.h"
#include "dtOVMMesh.h"
#include <interfaceHeaven/barChart.h>
#include "dtOptimizeMeshGRegion.h"
#include <progHelper.h>

namespace dtOO {
  dtMeshGRegion::dtMeshGRegion() : dtMesh3DOperator() {
  }

  dtMeshGRegion::dtMeshGRegion(
    const dtMeshGRegion& orig
  ) : dtMesh3DOperator(orig) {
    _relax = orig._relax;
    _minQShapeMetric = orig._minQShapeMetric;
    _nPyramidOpenSteps = orig._nPyramidOpenSteps;
    _nSmooths = orig._nSmooths;
  }

  dtMeshGRegion::~dtMeshGRegion() {
  }
  
  void dtMeshGRegion::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,
    bVPtrVec const * const bV,
    labeledVectorHandling< dtMeshOperator * > const * const mO
  ) {
    dtMesh3DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _relax 
    = 
    qtXmlBase::getAttributeFloatMuParse("relax", element, cV, aF);
    _minQShapeMetric 
    = 
    qtXmlBase::getAttributeFloatMuParse("minQShapeMetric", element, cV, aF);    
    _nPyramidOpenSteps
    = 
    qtXmlBase::getAttributeIntMuParse("nPyramidOpenSteps", element, cV, aF);     
    _nSmooths
    = 
    qtXmlBase::getAttributeIntMuParse("nSmooths", element, cV, aF);   
  }

  void dtMeshGRegion::operator()( dtGmshRegion * dtgr) {
    //
    // transfinite meshing
    //
    if ( dtgr->meshAttributes.method == MESH_TRANSFINITE ) {
      std::vector< ::GRegion * > delauny;
      ::meshGRegion mr( delauny );
      mr(dtgr);
      MeshDelaunayVolume(delauny);
      
      dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;      
      
      return;
    }
       
    //
    // get number of quad faces
    //
    int quadFaces = 0;
    int quads = 0;
    int elems = 0;
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshFace( dtgr->faces() ), gf ) {
      quads = quads + gf->quadrangles.size();
      elems = elems + gf->getNumMeshElements();
      if ( gf->quadrangles.size() ) {
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
      dtOptimizeMeshGRegion()(dtgr);

      dtgr->_status = ::GEntity::MeshGenerationStatus::DONE;
    }
  }
  
  void dtMeshGRegion::createPyramids( dtGmshRegion * dtgr ) {   
    logContainer< dtMeshGRegion > logC(logINFO, "createPyramids()");
    
    std::map< dtGmshFace *, dtGmshFace * > pseudo_org;
    std::vector< ::MPyramid * > pyramids;
    std::vector< ::MVertex * > vertices;    
    
    dt__forAllRefAuto( dtGmshModel::cast2DtGmshFace( dtgr->faces() ), gf ) {
      //
      // modify only quadrangle surfaces
      //
      if (gf->quadrangles.size() == 0) continue;

      logC()
        << "Face " << gf->tag() << " contains quadrangles." << std::endl
        << "Remove face temporarily." << std::endl;

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
        // create new pseudo mesh elements (triangles)
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

        //
        // create new mesh vertex
        //
        vertices.push_back( new ::MVertex( bb.x(), bb.y(), bb.z(), pseudo ) );
        pseudo->addMeshVertex(vertices.back());
        
        //
        // create new pseudo mesh elements (triangles)
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
    
    logC()
      << "Region: " << std::endl
      << dtgr->dumpToString()
      << std::endl;
    
    //
    // call meshing again
    //
    this->operator()(dtgr);
    
    //
    // add mesh vertices and pyramids to old volume
    //
    std::map< ::MVertex *, float > minQ_mv;
    dt__forAllRefAuto(vertices, aVert) {
      aVert->setEntity(dtgr);
      dtgr->addMeshVertex(aVert);
      minQ_mv[ aVert ] = std::numeric_limits<float>::min();
    }
    dt__forAllRefAuto(pyramids, aPyr) dtgr->addPyramid( aPyr );
    
    //
    // delete created pseudo GFace
    //
    dt__forAllRefAuto(pseudo_org, aPair) {
      //
      // clear mesh
      //
      aPair.second->mesh_vertices.clear();
      aPair.second->deleteMesh();
      
      //
      // replace face
      //
      dtgr->replaceFace(aPair.second, aPair.first);

      //
      // remove in model
      //
      dtgr->model()->remove(aPair.second);
    }
    
    //
    // create barChart
    //
    barChart QTet_0("QTet_0", -1., 1., 30);
    dt__forAllRefAuto(dtgr->tetrahedra, aTet) QTet_0( qShapeMetric()(aTet) );
    dt__info(createPyramids(), << QTet_0);
          
    dtOVMMesh ovm;
    createOVM(dtgr, ovm);
    
    //
    // first open
    //
    dt__forAllRefAuto(vertices, aVert) {
      ovmVertexH const & vH = ovm.at( aVert );
      dt__throwIf(!vH.is_valid(), createPyramids());

      dtPoint3 cC = dtGmshModel::extractPosition(aVert);

      //
      // set vertex position
      //
      std::vector< dtPoint3 > pp = ovm.adjacentVertices(vH);
      dtPoint3 c1
      = 
      dtLinearAlgebra::toDtPoint3( 
        (1./pp.size()) 
        * 
        dtLinearAlgebra::sum( dtLinearAlgebra::toDtVector3(pp) )
      );
      ovm.replacePosition( vH,  cC + 0.01 * (c1 - cC) );
    }
    dt__forAllRefAuto(pyramids, aPyr) {
      if (aPyr->getVolume() < 0.) aPyr->reverse();
    }
        
    //
    // get minimal shape metric
    //
    float gMinQ = std::numeric_limits<float>::max();
    dt__forAllRefAuto(dtgr->tetrahedra, aTet) {
      gMinQ = std::min( qShapeMetric()( aTet ), gMinQ );
    }
    dt__forAllRefAuto(dtgr->pyramids, aPyr) {
      gMinQ = std::min( qShapeMetric()( aPyr ), gMinQ );
    }
    
    //
    // pyramid open method
    //
    dt__forFromToIndex(0, _nPyramidOpenSteps, ii) {
      int vertMove = 0;
      int vertFix = 0;
      dt__forAllRefAuto(vertices, aVert) {
        ovmVertexH const & vH = ovm.at( aVert );
        dt__throwIf(!vH.is_valid(), createPyramids());

        dtPoint3 cC = dtGmshModel::extractPosition(aVert);

        //
        // set vertex position
        //
        std::vector< dtPoint3 > pp = ovm.adjacentVertices(vH);
        dtPoint3 c1
        = 
        dtLinearAlgebra::toDtPoint3( 
          (1./pp.size()) 
          * 
          dtLinearAlgebra::sum( dtLinearAlgebra::toDtVector3(pp) )
        );
        ovm.replacePosition( vH,  cC + _relax * (c1 - cC) );

        float pyrShape = std::numeric_limits<float>::min();
        
        std::vector< float > qq;
        for(
          ovmVertexCellI vcIt = ovm.vc_iter(vH); vcIt.valid(); ++vcIt
        ) {
          qq.push_back( qShapeMetric()( ovm.at(*vcIt) ) );          

          // tetrahedra
          if ( ovm.at(*vcIt)->getNumVertices() == 4 ) { 
            //
            // detect volume sign change --> element gets inverted
            //
            float vol = ovm[ *vcIt ]->getVolume();
            float iVol = ovm.request_cell_property< float >("iV")[*vcIt];
            if ( ( iVol * vol ) <= 0. ) {
              qq[ qq.size() - 1 ] = -1. * fabs(qq[ qq.size() - 1 ]);
            }
          }
          // pyramid
          else pyrShape = qq.back();
        }

        //
        // check if step is not ok
        //        
        if ( 
          (
            (progHelper::min(qq) < gMinQ)// min shape metric should not decrease
            && 
            (pyrShape < _minQShapeMetric)// check pyramid shape metric
          )
          ||
          (progHelper::min(qq)<0.)// inverted element
        ) {
          // revert last step
          ovm.replacePosition( vH,  cC );
          vertFix++;
        }
        else {
          gMinQ = std::min( progHelper::min(qq), gMinQ );
          minQ_mv[ aVert ] = progHelper::min(qq);
          vertMove++;
        }
      }
      logC() 
        << logMe::dtFormat("%3i / %3i : %8i / %8i => Q = %8.2e")
          % ii % _nPyramidOpenSteps % vertMove % vertFix % gMinQ
        << std::endl;
    }
    barChart QTet_1("QTet_1", -1., 1., 30);
    dt__forAllRefAuto(dtgr->tetrahedra, aTet) {
      QTet_1( fabs(qShapeMetric()(aTet)) );
    }
    dt__info(createPyramids(), << QTet_1);    
    
    barChart QPyr_1("QPyr_1", -1., 1., 30);
    dt__forAllRefAuto(dtgr->pyramids, aPyr) {
      QPyr_1( fabs(qShapeMetric()(aPyr)) );
    }
    dt__info(createPyramids(), << QPyr_1);
    
    dt__forFromToIndex(0, _nSmooths, ii) dtOptimizeMeshGRegion()(dtgr);
              
    barChart QTet_2("QTet_2", -1., 1., 30);
    dt__forAllRefAuto(dtgr->tetrahedra, aTet) {
      QTet_2( fabs(qShapeMetric()(aTet)) );
    }
    dt__info(createPyramids(), << QTet_2);    
    
    barChart QPyr_2("QPyr_2", -1., 1., 30);
    dt__forAllRefAuto(dtgr->pyramids, aPyr) {
      QPyr_2( fabs(qShapeMetric()(aPyr)) );
    }
    dt__info(createPyramids(), << QPyr_2);
  }
  
  void  dtMeshGRegion::createOVM(dtGmshRegion * dtgr, dtOVMMesh & ovm) {
    //
    // create overall element vector
    //
    std::vector< ::MElement * > me(dtgr->getNumMeshElements());
    dt__forFromToIndex(0, dtgr->getNumMeshElements(), ii) {
     me[ii] = dtgr->getMeshElement(ii);
    }
    
    //
    // create octree
    //
    ::MElementOctree oct(me);
    
    //
    // create OpenVolumeMesh
    //
    ::OpenVolumeMesh::CellPropertyT< float > iV 
    = 
    ovm.request_cell_property< float >("iV");
    ovm.set_persistent( iV );
    
    int zeroVol = 0;
    dt__forAllRefAuto(dtgr->pyramids, aPyr) {
      dt__forFromToIndex(0, 5, ii) {
        std::vector< ::MElement * > meVec 
        = 
        oct.findAll(
          aPyr->getVertex(ii)->x(), 
          aPyr->getVertex(ii)->y(), 
          aPyr->getVertex(ii)->z(), 
          -1
        );       
        dt__forAllRefAuto(meVec, aMe) {
          if ( aMe->getVolume()==0. ) zeroVol++;
          ovmCellH cH = ovm.addCell(aMe);
          ovm.request_cell_property< float >("iV")[cH] = aMe->getVolume();
        }
      }
    }
  }
}

