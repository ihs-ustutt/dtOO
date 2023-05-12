#ifndef bVOAnalyticFunctionToFace_H
#define	bVOAnalyticFunctionToFace_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class scaThreeD;
  
  class bVOAnalyticFunctionToFace : public bVOInterface {
    public:
      dt__class(bVOAnalyticFunctionToFace, bVOInterface);
      dt__classSelfCreate(bVOAnalyticFunctionToFace);
      bVOAnalyticFunctionToFace();
      virtual ~bVOAnalyticFunctionToFace();
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
      dt__pVH(scaThreeD) _sca;
      static bool _registrated;
  };
}
#endif	/* bVOAnalyticFunctionToFace_H */

