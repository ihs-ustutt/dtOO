#ifndef TRANSINTCUBE_H
#define	TRANSINTCUBE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class map2dTo3d;
  
  class transIntCube {
  public:
    dt__classOnlyName(transIntCube);
    transIntCube();
    transIntCube(
      map2dTo3d const * const aS0, 
      map2dTo3d const * const aS1,
      map2dTo3d const * const aS2, 
      map2dTo3d const * const aS3, 
      map2dTo3d const * const aS4,
      map2dTo3d const * const aS5
    );
    transIntCube(const transIntCube& orig);
    virtual ~transIntCube();
    dtPoint3 getValue(float const xx, float const yy, float const zz) const;
    vectorHandling< map2dTo3d const * > const & getConstRefToMap2dTo3d( void ) const;
  private:
    vectorHandling< map2dTo3d const * > _aS;
    dtVector3 _aS_1_0_0;
    dtVector3 _aS_1_0_1;
    dtVector3 _aS_1_1_0;
    dtVector3 _aS_1_1_1;
    dtVector3 _aS_3_0_0;
    dtVector3 _aS_3_0_1;
    dtVector3 _aS_3_1_0;
    dtVector3 _aS_3_1_1;
  };
}
#endif	/* TRANSINTCUBE_H */

