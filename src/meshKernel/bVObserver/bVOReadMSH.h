#ifndef bVOReadMSH_H
#define	bVOReadMSH_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOReadMSH : public bVOInterface {
    public:
      dt__class(bVOReadMSH, bVOInterface);
      dt__classSelfCreate(bVOReadMSH);
      bVOReadMSH();
      virtual ~bVOReadMSH();
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
#endif	/* bVOReadMSH_H */

