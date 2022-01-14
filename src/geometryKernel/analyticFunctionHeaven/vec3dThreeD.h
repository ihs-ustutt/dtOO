#ifndef VEC3DTHREED_H
#define	VEC3DTHREED_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dFunction.h"
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class renderInterface;
  class dtTransformer;
  
  class vec3dThreeD : public vec3dFunction {
  public:
    dt__class(vec3dThreeD, analyticFunction);     
    vec3dThreeD();
    vec3dThreeD(const vec3dThreeD& orig);
    virtual ~vec3dThreeD();
    virtual vec3dThreeD * clone( void ) const = 0;
    virtual vec3dThreeD * cloneTransformed( 
      dtTransformer const * const dtT 
    ) const = 0;
    virtual vec3dThreeD * create( void ) const = 0;    
    virtual aFY Y( aFX const & xx ) const = 0;
    virtual aFY Y( dtReal const & x0, dtReal const & x1, dtReal const & x2 ) const;
    virtual int xDim( void ) const;
    void setMax(int const & dir, dtReal const & max);
    void setMin(int const & dir, dtReal const & min);
    using analyticFunction::xMin;
    using analyticFunction::xMax;
    virtual dtReal xMin( int const & dir) const;
    virtual dtReal xMax( int const & dir) const;   
    virtual std::vector<dtVector3> DYdtVector3( aFX const & xx ) const;
	  using analyticFunction::x_percent;
    using analyticFunction::percent_x;
	  aFX x_percent(dtReal const & x0, dtReal const & x1, dtReal const & x2) const;
	  aFX percent_x(dtReal const & x0, dtReal const & x1, dtReal const & x2) const;
    dtPoint3 YdtPoint3(aFX const & xx) const;
    dtPoint3 YdtPoint3(
      dtReal const & x0, dtReal const & x1, dtReal const & x2
    ) const;
    dtPoint3 YdtPoint3Percent(aFX const & xx) const;
    dtPoint3 YdtPoint3Percent(
      dtReal const & x0, dtReal const & x1, dtReal const & x2
    ) const;    
    virtual vectorHandling< renderInterface * > getRender( void ) const;    
  private:    
    dtReal _min[3];
    dtReal _max[3];
  };
  dt__H_addCloneForpVH(vec3dThreeD);
}
#endif	/* VEC3DTHREED_H */

