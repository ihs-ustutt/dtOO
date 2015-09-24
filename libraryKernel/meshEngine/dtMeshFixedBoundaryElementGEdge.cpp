#include "dtMeshFixedBoundaryElementGEdge.h"

#include "dtGmshEdge.h"
#include <analyticFunctionHeaven/aFBuilder/float_scaOneDPoint.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <xmlHeaven/qtXmlBase.h>
#include <gmsh/GmshDefines.h>
#include <gmsh/MVertex.h>
#include <gmsh/GVertex.h>

namespace dtOO {
  dtMeshFixedBoundaryElementGEdge::dtMeshFixedBoundaryElementGEdge(
  ) : dtMesh1DOperator() {
    
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
    dtMesh1DOperator::init(element, bC, cV, aF, aG, bV, mO);
    
    _typeTransfinite = qtXmlBase::getAttributeInt("typeTransfinite", element);
    _grading.reset(
      scaOneD::ConstDownCast(
        aF->get(qtXmlBase::getAttributeStr("gradingLabel", element))->clone()
      )
    );
    dt__throwIf(_grading.isNull(), init());
  }

  void dtMeshFixedBoundaryElementGEdge::operator()( dtGmshEdge * dtge) {
    if (
      dtge->meshAttributes.method == MESH_TRANSFINITE
      &&
      dtge->meshAttributes.typeTransfinite == _typeTransfinite
    ) {
      ::Msg::Info(
        "Meshing curve %d ( dtMeshFixedBoundaryElementGEdge transType=%d)", 
        dtge->tag(), _typeTransfinite
      );      
      map1dTo3d const * const m1d = dtge->getMap1dTo3d();
      float ll = m1d->length();
      
      int nP = dtge->meshAttributes.nbPointsTransfinite;
      std::vector< float > gg = float_scaOneDPoint(_grading.get(), nP).result();
      if (dtge->meshAttributes.coeffTransfinite < 1.) {
        dt__forAllRefAuto(gg, aGG) aGG = 1.-aGG;
        progHelper::reverse(gg);
      }      
      dt__forFromToIndex(1, nP-1, ii) {
        float uu = m1d->u_l( gg[ii] * ll );
        dtPoint3 pp( m1d->getPoint(uu) );
        
        dtge->mesh_vertices.push_back( 
          new ::MEdgeVertex(
            pp.x(), pp.y(), pp.z(), dtge, static_cast<double>(uu)
          )
        );
      }
      dtge->meshStatistics.status = ::GEntity::MeshGenerationStatus::DONE;
    }
  }
}

