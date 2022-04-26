#ifndef bVOWriteMSH_H
#define	bVOWriteMSH_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOWriteMSH : public bVOInterface {
    public:
      dt__class(bVOWriteMSH, bVOInterface);
      dt__classSelfCreate(bVOWriteMSH);
      bVOWriteMSH();
      virtual ~bVOWriteMSH();
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
      std::string _filename;
      bool _saveAll;
      bool _saveParametric;
      static bool _registrated;
  };
}
#endif	/* bVOWriteMSH_H */

