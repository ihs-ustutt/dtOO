#ifndef dtPoint3_vec3dTwoD_H
#define	dtPoint3_vec3dTwoD_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class vec3dTwoD;
  
  class dtPoint3_vec3dTwoD {
  public:
    dt__classOnlyName(dtPoint3_vec3dTwoD);    
    dtPoint3_vec3dTwoD(
      vec3dTwoD const * const v3d, int const & nU, int const & nV
    );
    virtual ~dtPoint3_vec3dTwoD();
    twoDArrayHandling< dtPoint3 > result( void );
  private:
    twoDArrayHandling< dtPoint3 > _pp;
  };
}
#endif	/* dtPoint3_vec3dTwoD_H */

