#ifndef trans6SidedCube_splitTrans6SidedCube_H
#define	trans6SidedCube_splitTrans6SidedCube_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class trans6SidedCube;
  class analyticGeometry;
  
  class trans6SidedCube_splitTrans6SidedCube {
  public:
    dt__classOnlyName(trans6SidedCube_splitTrans6SidedCube);
    trans6SidedCube_splitTrans6SidedCube(
      trans6SidedCube const * const t6, dtPoint3 const & splitPoint
    );
    virtual ~trans6SidedCube_splitTrans6SidedCube();
    vectorHandling< trans6SidedCube * > result( void );
    vectorHandling< analyticGeometry * > resultGeometries( void );
  private:
    dt__pVH(trans6SidedCube) _t6;
    dt__pVH(analyticGeometry) _aG;
  };
}
#endif	/* trans6SidedCube_splitTrans6SidedCube_H */

