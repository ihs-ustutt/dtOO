#ifndef bVOCoherenceCheck_H
#define	bVOCoherenceCheck_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "bVOInterface.h"
#include <vector>

namespace dtOO {
  class boundedVolume;
  class dtTransformer;
  
  class bVOCoherenceCheck : public bVOInterface {
    public:
      dt__class(bVOCoherenceCheck, bVOInterface);
      dt__classSelfCreate(bVOCoherenceCheck);
      bVOCoherenceCheck();
      virtual ~bVOCoherenceCheck();
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
      dtReal _relTol;
      dtReal _absTol;
      static bool _registrated;
  };
}
#endif	/* bVOCoherenceCheck_H */

