#ifndef DTMESH2DOPERATOR_H
#define	DTMESH2DOPERATOR_H

#include <dtOOTypeDef.h>

#include "dtMeshOperator.h"

class QDomElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtGmshFace;
  
  class dtMesh2DOperator : public dtMeshOperator {
    public:
      dt__class(dtMesh2DOperator, dtMeshOperator);    
      dtMesh2DOperator();
      dtMesh2DOperator(const dtMesh2DOperator& orig);
      virtual ~dtMesh2DOperator();
      virtual dtMesh2DOperator * create( void ) const = 0;     
      virtual void jInit(
        jsonPrimitive const & jE,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO
      );
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO      
      );
      virtual void operator()( dtGmshFace * dtgf ) = 0;    
      static void copyMesh( dtGmshFace * from, dtGmshFace * to );        
  };
  dt__I_addCloneForpVHNotImpl(dtMesh2DOperator);
}
#endif	/* DTMESH2DOPERATOR_H */

