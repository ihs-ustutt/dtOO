#include "dtMesh2DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include "dtGmshFace.h"
#include <gmsh/meshGEdge.h>
void copyMesh(GFace *source, GFace *target);

namespace dtOO {
  dtMesh2DOperator::dtMesh2DOperator() {
  }

  dtMesh2DOperator::dtMesh2DOperator(const dtMesh2DOperator& orig) {
  }

  dtMesh2DOperator::~dtMesh2DOperator() {
  }

  void dtMesh2DOperator::jInit(
    jsonPrimitive const & jE,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtMeshOperator const * const mO    
  ) {
    dtMeshOperator::jInit(jE, bC, cV, aF, aG, bV, mO);
  }

  void dtMesh2DOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
    lvH_analyticGeometry const * const aG,
    lvH_boundedVolume const * const bV,
    lvH_dtMeshOperator const * const mO    
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }

  void dtMesh2DOperator::copyMesh( dtGmshFace * from, dtGmshFace *to) {
    Msg::Info(
      "Copy mesh from surface %d to surface %d ( dtMesh2DOperator )", 
      from->tag(), to->tag()
    );
    ::copyMesh( (::GFace *) from, (::GFace *) to );
  }
}
