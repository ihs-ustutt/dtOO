#ifndef bVOWriteSTL_H
#define	bVOWriteSTL_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOWriteSTL : public bVOInterface {
    public:
      dt__class(bVOWriteSTL, bVOInterface);
      dt__classSelfCreate(bVOWriteSTL);
      bVOWriteSTL();
      virtual ~bVOWriteSTL();
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
  };
}
#endif	/* bVOWriteSTL_H */

