#ifndef DTMESH2DOPERATOR_H
#define	DTMESH2DOPERATOR_H

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
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtMeshOperator * > const * const mO      
    );
    virtual void operator()( dtGmshFace * dtgf ) = 0;    
    static void copyMesh( dtGmshFace * from, dtGmshFace * to );        
  private:

  };
}
#endif	/* DTMESH2DOPERATOR_H */

