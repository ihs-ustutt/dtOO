#include "dtMeshFreeGradingGEdge.h"

#include "dtGmshEdge.h"
#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/scaMultiOneD.h>
#include <analyticFunctionHeaven/scaLinearOneD.h>
#include <xmlHeaven/qtXmlBase.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/MVertex.h>
#include <gmsh/GVertex.h>
#include <gmsh/MLine.h>
#include <interfaceHeaven/timeHandling.h>
#include <interfaceHeaven/threadSafe.h>
#include <omp.h>
#include <progHelper.h>

namespace dtOO {
  dtMeshFreeGradingGEdge::dtMeshFreeGradingGEdge(
  ) : dtMeshGEdge() {
    
  }

  dtMeshFreeGradingGEdge::~dtMeshFreeGradingGEdge() {
    
  } 
    
  void dtMeshFreeGradingGEdge::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO      
  ) {
    dtMeshGEdge::init(element, bC, cV, aF, aG, bV, mO);
    
    _typeTransfinite 
    = 
    qtXmlBase::getAttributeIntVector("typeTransfinite", element);
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
    
    _tol = 1.e-8;
    if ( qtXmlBase::hasAttribute("tolerance", element) ) {
      _tol = qtXmlBase::getAttributeFloatMuParse("tolerance", element, cV);
    }    
    _maxSmooth = 20;
    if ( qtXmlBase::hasAttribute("nSmoothSteps", element) ) {
      _maxSmooth 
      = 
      qtXmlBase::getAttributeIntMuParse("nSmoothSteps", element, cV);
    }      
  }

  void dtMeshFreeGradingGEdge::operator()( dtGmshEdge * dtge) {
    int transType = dtge->meshAttributes.typeTransfinite;
    if (
      dtge->meshAttributes.method == MESH_TRANSFINITE
      &&
      _gradingInt.find(transType) != _gradingInt.end()
    ) {
      ::Msg::Info(
        "Meshing curve %d ( dtMeshFreeGradingGEdge transType=%d)", 
        dtge->tag(), transType
      );      
      map1dTo3d const * const m1d = dtge->getMap1dTo3d();
      
      int nP = dtge->meshAttributes.nbPointsTransfinite;
      std::vector< float > gg 
      = 
      float_scaOneDPoint(_gradingInt[transType], nP).result();
      if (dtge->meshAttributes.coeffTransfinite < 1.) {
        dt__forAllRefAuto(gg, aGG) aGG = 1.-aGG;
        progHelper::reverse(gg);
      }

      logContainer< dtMeshFreeGradingGEdge > logC(logDEBUG, "operator()");
      
      std::vector< float > uu(gg.size());
      uu[0] = m1d->getUMin();
      uu[gg.size()-1] = m1d->getUMax();
      std::vector< dtPoint3 > p3_u(gg.size());
      p3_u[0] = m1d->getPoint(uu[0]);
      p3_u[gg.size()-1] = m1d->getPoint( uu[gg.size()-1] );

      dt__forFromToIndex(1, nP, ii) uu[ii] = m1d->u_percent( gg[ii] );
      
//      #pragma omp parallel 
//      {
//        threadSafe< dt__pH(map1dTo3d) > wMap;
//        wMap().reset( m1d->clone() );
//        #pragma omp for
//        dt__forFromToIndex(1, nP-1, ii) {
//          uu[ii] = wMap()->u_percent( gg[ii] );
//          p3_u[ii] = wMap()->getPoint(uu[ii]);
//        }     
//      }

      
      dt__forFromToIndex(0, _maxSmooth, smoothIt) {
        std::vector< float > dL(gg.size(), 0.);      
        dt__forFromToIndex(1, nP, ii) {
          p3_u[ii] = m1d->getPoint(uu[ii]);
          dL[ii] = dtLinearAlgebra::length( p3_u[ii] - p3_u[ii-1] );
        }     


        float sumL = dtLinearAlgebra::sum(dL);
        std::vector< float > ll(gg.size(), 0.);      
        scaMultiOneD< scaLinearOneD > l_u;
        dt__forFromToIndex(1, nP, ii) {
          ll[ii] = ll[ii-1] + dL[ii];        
          l_u.add( scaLinearOneD(uu[ii-1], uu[ii], ll[ii-1], ll[ii]) );
        }  

//        logC() 
//          << "sumL = " << sumL << std::endl
//          << logMe::dtFormat("%16s %16s %16s %16s %16s %16s => %16s") 
//            % "u_n" % "l-l_0" % "dL" % "gg" % "(l-l_0)/sum(l)" 
//            % "gg-(l-l_0)/sum(l)" % "u_n+1"
//          << std::endl;
        float sumEps = 0;
        float maxEps = std::numeric_limits<float>::min();
        dt__forFromToIndex(1, nP-1, ii) {
//          logC() 
//            << logMe::dtFormat(
//              "%16.10e %16.10e %16.10e %16.10e %16.10e %16.10e => %16.10e"
//            ) 
//              % uu[ii] % ll[ii] % dL[ii] % gg[ii] % (ll[ii]/sumL) 
//              % (gg[ii]-(ll[ii]/sumL)) % l_u.invYFloat( gg[ii] * sumL )
//            << std::endl;
          sumEps = sumEps + fabs(gg[ii]-(ll[ii]/sumL));
          maxEps = std::max( maxEps, std::fabs<float>(gg[ii]-(ll[ii]/sumL)) );
          uu[ii] = l_u.invYFloat( gg[ii] * sumL );
        }

        logC() 
          << logMe::dtFormat(
            "[ %3i ] sumEps = %12.5e maxEps = %12.5e (threshold = %12.5e)"
          ) % smoothIt % sumEps % maxEps % _tol 
          << std::endl;
        
        if (maxEps < _tol) break;
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
      dtge->addLine( 
        new ::MLine( 
          dtge->getBeginVertex()->mesh_vertices[0], dtge->mesh_vertices.front()
        )
      );        
      dt__forFromToIndex(1, dtge->mesh_vertices.size(), ii) {
        dtge->addLine( 
          new ::MLine( 
            dtge->mesh_vertices[ii-1], dtge->mesh_vertices[ii] 
          )
        );        
      }
      dtge->addLine( 
        new ::MLine( 
          dtge->mesh_vertices.back(), dtge->getEndVertex()->mesh_vertices[0]
        )
      );                    
      dtge->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
    }
    else dtMeshGEdge::operator()( dtge );
  }
}

