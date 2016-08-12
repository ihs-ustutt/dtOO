#ifndef bVOSetPrescribedMeshSizeAtPoints_H
#define	bVOSetPrescribedMeshSizeAtPoints_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  
  class bVOSetPrescribedMeshSizeAtPoints : public bVOInterface {
  public:
    dt__class(bVOSetPrescribedMeshSizeAtPoints, bVOInterface);
    dt__classSelfCreate(bVOSetPrescribedMeshSizeAtPoints);
    bVOSetPrescribedMeshSizeAtPoints();
    virtual ~bVOSetPrescribedMeshSizeAtPoints();
    virtual void init(
      ::QDomElement const & element,
      baseContainer const * const bC,
      vectorHandling< constValue * > const * const cV,
      vectorHandling< analyticFunction * > const * const aF,
      vectorHandling< analyticGeometry * > const * const aG,
      vectorHandling< boundedVolume * > const * const bV,
      boundedVolume * attachTo
    );    
    virtual void preUpdate( void );
  private:
    float _meshSize;
  };
}
#endif	/* bVOSetPrescribedMeshSizeAtPoints_H */

