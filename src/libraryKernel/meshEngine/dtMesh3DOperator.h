#ifndef DTMESH3DOPERATOR_H
#define	DTMESH3DOPERATOR_H

#include <dtOOTypeDef.h>

#include "dtMeshOperator.h"

class QDomElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtGmshRegion;
  
  class dtMesh3DOperator : public dtMeshOperator {
    public:
      dt__class(dtMesh3DOperator, dtMeshOperator);    
      dtMesh3DOperator();
      dtMesh3DOperator(const dtMesh3DOperator& orig);
      virtual ~dtMesh3DOperator();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        labeledVectorHandling< dtMeshOperator * > const * const mO      
      );
      virtual void operator()( dtGmshRegion * dtgr ) = 0;    
  };
  dt__I_addCloneForpVHNotImpl(dtMesh3DOperator);
}
#endif	/* DTMESH3DOPERATOR_H */

