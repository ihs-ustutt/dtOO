#ifndef bVOAnalyticGeometryToFace_H
#define	bVOAnalyticGeometryToFace_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include "bVOInterface.h"

namespace dtOO {
  class constValue;
  class analyticFunction;
  class analyticGeometry;
  class map2dTo3d;
  
  class bVOAnalyticGeometryToFace : public bVOInterface {
    public:
      dt__class(bVOAnalyticGeometryToFace, bVOInterface);
      dt__classSelfCreate(bVOAnalyticGeometryToFace);
      bVOAnalyticGeometryToFace();
      virtual ~bVOAnalyticGeometryToFace();
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
  //    std::vector< std::string > _patchRule;
      dt__pVH(map2dTo3d) _m2d;
      dtReal _inc;
      static bool _registrated;
  };
}
#endif	/* bVOAnalyticGeometryToFace_H */

