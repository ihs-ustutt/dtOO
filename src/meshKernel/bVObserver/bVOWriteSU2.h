#ifndef bVOWriteSU2_H
#define	bVOWriteSU2_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOWriteSU2 : public bVOInterface {
    public:
      dt__class(bVOWriteSU2, bVOInterface);
      dt__classSelfCreate(bVOWriteSU2);
      bVOWriteSU2();
      virtual ~bVOWriteSU2();
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
      static bool _registrated;      
  };
}
#endif	/* bVOWriteSU2_H */

