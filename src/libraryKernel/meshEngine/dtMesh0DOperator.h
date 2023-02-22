#ifndef dtMesh0DOperator_H
#define	dtMesh0DOperator_H

#include <dtOOTypeDef.h>

#include "dtMeshOperator.h"

class QDomElement;

namespace dtOO {
  class baseContainer;
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class boundedVolume;
  class dtGmshVertex;
  
  class dtMesh0DOperator : public dtMeshOperator {
    public:
      dt__class(dtMesh0DOperator, dtMeshOperator);    
      dtMesh0DOperator();
      virtual ~dtMesh0DOperator();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        lvH_dtMeshOperator const * const mO      
      );
      virtual void operator()( dtGmshVertex * dtgv ) = 0;   
      static void correspondingVertex( 
        dtGmshVertex * master, dtGmshVertex * slave  
      );    
  };
  dt__I_addCloneForpVHNotImpl(dtMesh0DOperator);
}
#endif	/* dtMesh0DOperator_H */

