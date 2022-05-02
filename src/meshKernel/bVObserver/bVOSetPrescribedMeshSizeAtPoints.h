#ifndef bVOSetPrescribedMeshSizeAtPoints_H
#define	bVOSetPrescribedMeshSizeAtPoints_H

#include <dtOOTypeDef.h>

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
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::vector< dtReal > _meshSize;
      static bool _registrated;
  };
}
#endif	/* bVOSetPrescribedMeshSizeAtPoints_H */

