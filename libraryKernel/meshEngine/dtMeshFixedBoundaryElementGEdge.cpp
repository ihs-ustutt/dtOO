#include "dtMeshFixedBoundaryElementGEdge.h"

#include "dtGmshEdge.h"
#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <xmlHeaven/qtXmlBase.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/MVertex.h>
#include <gmsh/GVertex.h>
#include <gmsh/MLine.h>
#include <interfaceHeaven/timeHandling.h>
#include <omp.h>

namespace dtOO {
  dtMeshFixedBoundaryElementGEdge::dtMeshFixedBoundaryElementGEdge(
  ) : dtMeshGEdge() {
    
  }

  dtMeshFixedBoundaryElementGEdge::~dtMeshFixedBoundaryElementGEdge() {
    
  } 
    
  void dtMeshFixedBoundaryElementGEdge::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO      
  ) {
    dtMeshGEdge::init(element, bC, cV, aF, aG, bV, mO);
    
    _typeTransfinite = qtXmlBase::getAttributeIntVector("typeTransfinite", element);
    std::vector< std::string > aFLabel 
    = 
    qtXmlBase::getAttributeStrVector("gradingLabel", element);
    dt__forAllRefAuto(aFLabel, aLabel) {
      dt__ptrAss( 
        scaOneD const * theF, scaOneD::ConstDownCast(aF->get(aLabel)) 
      );
      _grading.push_back( theF->clone() );
    }
    dt__forAllIndex(_typeTransfinite, ii) {
      _gradingInt[ _typeTransfinite[ii] ] = &(_grading[ ii ]);
    }
  }

  void dtMeshFixedBoundaryElementGEdge::operator()( dtGmshEdge * dtge) {
    int transType = dtge->meshAttributes.typeTransfinite;
    if (
      dtge->meshAttributes.method == MESH_TRANSFINITE
      &&
      _gradingInt.find(transType) != _gradingInt.end()
    ) {
      ::Msg::Info(
        "Meshing curve %d ( dtMeshFixedBoundaryElementGEdge transType=%d)", 
        dtge->tag(), transType
      );      
      map1dTo3d const * const m1d = dtge->getMap1dTo3d();
      float ll = m1d->length();
      
      int nP = dtge->meshAttributes.nbPointsTransfinite;
      std::vector< float > gg = float_scaOneDPoint(_gradingInt[transType], nP).result();
      if (dtge->meshAttributes.coeffTransfinite < 1.) {
        dt__forAllRefAuto(gg, aGG) aGG = 1.-aGG;
        progHelper::reverse(gg);
      }

      std::vector< float > uu(gg.size());
      #pragma omp parallel 
      {
        #pragma omp for
        dt__forFromToIndex(1, nP-1, ii) {
          uu[ii] = m1d->u_l( gg[ii] * ll );
        }
      }
      
      //
      // add mesh vertices
      //
      dt__forFromToIndex(1, nP-1, ii) {
        dtPoint3 pp( m1d->getPoint(uu[ii]) );
        
        dtge->mesh_vertices.push_back( 
          new ::MEdgeVertex(pp.x(), pp.y(), pp.z(), dtge, uu[ii])
        );
      }
      
      //
      // add mesh elements
      //
      if ( dtge->meshAttributes.coeffTransfinite < 1. ) {
        dtge->addLine( 
          new ::MLine( 
            dtge->mesh_vertices.front(), dtge->getEndVertex()->mesh_vertices[0]
          )
        );
      }
      else {
        dtge->addLine( 
          new ::MLine( 
            dtge->getBeginVertex()->mesh_vertices[0], dtge->mesh_vertices.front()
          )
        );        
      }
      dt__forFromToIndex(1, dtge->mesh_vertices.size(), ii) {
        dtge->addLine( 
          new ::MLine( 
            dtge->mesh_vertices[ii-1], dtge->mesh_vertices[ii] 
          )
        );        
      }
      if ( dtge->meshAttributes.coeffTransfinite < 1. ) {
        dtge->addLine( 
          new ::MLine( 
            dtge->getBeginVertex()->mesh_vertices[0], dtge->mesh_vertices.back()
          )
        );            
      }
      else {
        dtge->addLine( 
          new ::MLine( 
            dtge->mesh_vertices.back(), dtge->getEndVertex()->mesh_vertices[0]
          )
        );                    
      }
      dtge->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
    }
    else dtMeshGEdge::operator()( dtge );
  }
}

