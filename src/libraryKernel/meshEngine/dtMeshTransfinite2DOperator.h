#ifndef dtMeshTransfinite2DOperator_H
#define	dtMeshTransfinite2DOperator_H

#include "dtMesh2DOperator.h"
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>

class QDomElement;

class GFace;
class MVertex;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtGmshFace;
  
  class dtMeshTransfinite2DOperator : public dtMesh2DOperator {
    public:
      dt__class(dtMeshTransfinite2DOperator, dtMeshOperator);    
      dtMeshTransfinite2DOperator();
      dtMeshTransfinite2DOperator(const dtMeshTransfinite2DOperator& orig);
      virtual ~dtMeshTransfinite2DOperator();
      virtual void operator()( dtGmshFace * dtgf ) = 0;
      static twoDArrayHandling< dtPoint2 > computeEdgeLoops( dtGmshFace * gf );
      static void createTransfiniteElements( dtGmshFace * const dtgf );
    private:

  };
}
#endif	/* dtMeshTransfinite2DOperator_H */

