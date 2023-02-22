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
        lvH_constValue const * const cV,
        lvH_analyticFunction const * const aF,
        lvH_analyticGeometry const * const aG,
        lvH_boundedVolume const * const bV,
        boundedVolume * attachTo
      );    
      virtual void postUpdate( void );
    private:
      static bool _registrated;
  };
}
#endif	/* bVOFaceToPatchRule_H */

