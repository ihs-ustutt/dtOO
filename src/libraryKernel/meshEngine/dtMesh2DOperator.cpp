#include "dtMesh2DOperator.h"

#include <xmlHeaven/qtXmlPrimitive.h>
#include "dtGmshFace.h"
#include <gmsh/meshGEdge.h>
void copyMesh(GFace *source, GFace *target, bool fastCopy);

namespace dtOO {
  dtMesh2DOperator::dtMesh2DOperator() {
  }

  dtMesh2DOperator::dtMesh2DOperator(const dtMesh2DOperator& orig) {
  }

  dtMesh2DOperator::~dtMesh2DOperator() {
  }

  void dtMesh2DOperator::init(
    ::QDomElement const & element,
    baseContainer const * const bC,
    vectorHandling< constValue * > const * const cV,
    vectorHandling< analyticFunction * > const * const aF,
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< boundedVolume * > const * const bV,
    vectorHandling< dtMeshOperator * > const * const mO    
  ) {
    dtMeshOperator::init(element, bC, cV, aF, aG, bV, mO);
  }

  void dtMesh2DOperator::copyMesh( dtGmshFace * from, dtGmshFace *to) {
    //
    //@todo: Should be checked if this fast copy method is ok!
    //
    ::copyMesh( (::GFace *) from, (::GFace *) to, true );
  }
}