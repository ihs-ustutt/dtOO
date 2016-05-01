#ifndef DTMESH3DOPERATOR_H
#define	DTMESH3DOPERATOR_H

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
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      vectorHandling< dtMeshOperator * > const * const mO      
    );
    virtual void operator()( dtGmshRegion * dtgr ) = 0;    
  private:

  };
}
#endif	/* DTMESH3DOPERATOR_H */

