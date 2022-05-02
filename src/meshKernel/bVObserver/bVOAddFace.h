#ifndef bVOAddFace_H
#define	bVOAddFace_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtGmshEdge;
  class map1dTo3d;
  
  class bVOAddFace : public bVOInterface {
    public:
      dt__class(bVOAddFace, bVOInterface);
      dt__classSelfCreate(bVOAddFace);
      bVOAddFace();
      virtual ~bVOAddFace();
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
      std::string _regionLabel;
      std::vector< std::string > _faceLabel;
      static bool _registrated;
  };
}
#endif	/* bVOAddFace_H */

