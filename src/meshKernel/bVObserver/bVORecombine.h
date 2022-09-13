#ifndef bVORecombine_H
#define	bVORecombine_H

#include <dtOOTypeDef.h>

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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void preUpdate( void );
    private:
      std::vector< std::string > _faceLabel;
      static bool _registrated;
  };
}
#endif	/* bVORecombine_H */

