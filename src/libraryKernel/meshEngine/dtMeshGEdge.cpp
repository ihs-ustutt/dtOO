#include "dtMeshGEdge.h"

#include "dtGmshEdge.h"
#include <gmsh/meshGEdge.h>
#include <gmsh/Context.h>
#include "dtMeshOperatorFactory.h"

namespace dtOO {
  bool dtMeshGEdge::_registrated 
  =
  dtMeshOperatorFactory::registrate(
    dt__tmpPtr(dtMeshGEdge, new dtMeshGEdge())
  );
 
  dtMeshGEdge::dtMeshGEdge() : dtMesh1DOperator() {
  }

  dtMeshGEdge::dtMeshGEdge(const dtMeshGEdge& orig) : dtMesh1DOperator(orig) {
  }

  dtMeshGEdge::~dtMeshGEdge() {
  } 

  void dtMeshGEdge::operator()( dtGmshEdge * dtge) {
    //
    // reset temporarily lc parameter of gmsh
    // --> prevents really slow meshing
    //
    double lcOld = CTX::instance()->lc;
    CTX::instance()->lc = 1.e+12 * CTX::instance()->mesh.lcIntegrationPrecision;
    ::meshGEdge()( dtge );
    CTX::instance()->lc = lcOld;
  }
}

