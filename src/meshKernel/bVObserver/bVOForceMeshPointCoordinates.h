#ifndef bVOForceMeshPointCoordinates_H
#define	bVOForceMeshPointCoordinates_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class boundedVolume;
  class vec3dThreeD;
  
  class bVOForceMeshPointCoordinates : public bVOInterface {
    public:
      dt__class(bVOForceMeshPointCoordinates, bVOInterface);
      dt__classSelfCreate(bVOForceMeshPointCoordinates);
      bVOForceMeshPointCoordinates();
      virtual ~bVOForceMeshPointCoordinates();
      virtual void init(
        ::QDomElement const & element,
        baseContainer const * const bC,
        cVPtrVec const * const cV,
        aFPtrVec const * const aF,
        aGPtrVec const * const aG,
        bVPtrVec const * const bV,
        boundedVolume * attachTo
      );    
      virtual void postUpdate( void );
    private:
      dt__pH(vec3dThreeD) _v3d;
      std::vector< std::string > _faceLabel;
      static bool _registrated;
  };
}
#endif	/* bVOForceMeshPointCoordinates_H */

