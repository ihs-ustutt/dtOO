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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      static bool _registrated;
  };
}
#endif	/* bVOSetPrescribedMeshSizeAtPoints_H */

