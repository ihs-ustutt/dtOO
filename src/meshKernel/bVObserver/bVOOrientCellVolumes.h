#ifndef bVOOrientCellVolumes_H
#define	bVOOrientCellVolumes_H

#include <dtOOTypeDef.h>

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
#endif	/* bVOOrientCellVolumes_H */

