#ifndef dtPoint3_map2dTo3dPoint_H
#define	dtPoint3_map2dTo3dPoint_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class map2dTo3d;
  
  class dtPoint3_map2dTo3dPoint {
  public:
    dt__classOnlyName(dtPoint3_map2dTo3dPoint);    
    dtPoint3_map2dTo3dPoint(
      map2dTo3d const * const m2d, dtInt const & nU, dtInt const & nV
    );
    virtual ~dtPoint3_map2dTo3dPoint();
    twoDArrayHandling< dtPoint3 > result( void );
  private:
    twoDArrayHandling< dtPoint3 > _pp;
  };
}
#endif	/* dtPoint3_map2dTo3dPoint_H */

