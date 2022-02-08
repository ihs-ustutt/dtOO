#ifndef bVOFaceToPatchRule_H
#define	bVOFaceToPatchRule_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOFaceToPatchRule : public bVOInterface {
    public:
      dt__class(bVOFaceToPatchRule, bVOInterface);
      dt__classSelfCreate(bVOFaceToPatchRule);
      bVOFaceToPatchRule();
      virtual ~bVOFaceToPatchRule();
      virtual std::vector< std::string > factoryAlias( void ) const;         
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
#endif	/* bVOFaceToPatchRule_H */

