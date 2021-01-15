#ifndef bVORecombine_H
#define	bVORecombine_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  
  class bVORecombine : public bVOInterface {
    public:
      dt__class(bVORecombine, bVOInterface);
      dt__classSelfCreate(bVORecombine);
      bVORecombine();
      virtual ~bVORecombine();
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
      std::vector< std::string > _faceLabel;
  };
}
#endif	/* bVORecombine_H */

