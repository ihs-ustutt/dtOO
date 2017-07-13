#ifndef bVOAnalyticFunctionToFace_H
#define	bVOAnalyticFunctionToFace_H

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
      cVPtrVec const * const cV,
      aFPtrVec const * const aF,
      aGPtrVec const * const aG,
      bVPtrVec const * const bV,
      boundedVolume * attachTo
    );    
    virtual void preUpdate( void );
  private:
    dt__pVH(scaThreeD) _sca;
  };
}
#endif	/* bVOAnalyticFunctionToFace_H */

