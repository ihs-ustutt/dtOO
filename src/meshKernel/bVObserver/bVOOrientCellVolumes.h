#ifndef bVOOrientCellVolumes_H
#define	bVOOrientCellVolumes_H

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  
  class bVOOrientCellVolumes : public bVOInterface {
    public:
      dt__class(bVOOrientCellVolumes, bVOInterface);
      dt__classSelfCreate(bVOOrientCellVolumes);
      bVOOrientCellVolumes();
      virtual ~bVOOrientCellVolumes();
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
      bool _positive;
  };
}
#endif	/* bVOOrientCellVolumes_H */

