#ifndef dtMesh0DOperator_H
#define	dtMesh0DOperator_H

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
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      labeledVectorHandling< dtMeshOperator * > const * const mO      
    );
    virtual void operator()( dtGmshVertex * dtgv ) = 0;   
    static void correspondingVertex( 
      dtGmshVertex * master, dtGmshVertex * slave  
    );    
  private:

  };
}
#endif	/* dtMesh0DOperator_H */
