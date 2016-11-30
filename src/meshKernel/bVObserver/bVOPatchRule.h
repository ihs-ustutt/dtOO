#ifndef bVOPatchRule_H
#define	bVOPatchRule_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOPatchRule : public bVOInterface {
  public:
    dt__class(bVOPatchRule, bVOInterface);
    dt__classSelfCreate(bVOPatchRule);
    bVOPatchRule();
    virtual ~bVOPatchRule();
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
    std::vector< std::string > _patchRule;
    std::vector< std::string > _regRule;
  };
}
#endif	/* bVOPatchRule_H */

