#ifndef VEC3DCURVE2DINSURFACEONED_H
#define	VEC3DCURVE2DINSURFACEONED_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec3dOneD.h"
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve2d;
  class dtSurface;
  class renderInterface;
  
  class vec3dCurve2dInSurfaceOneD : public vec3dOneD {
  public:
    dt__CLASSSTD(vec3dCurve2dInSurfaceOneD, analyticFunction);     
    vec3dCurve2dInSurfaceOneD();
    vec3dCurve2dInSurfaceOneD(const vec3dCurve2dInSurfaceOneD& orig);
    vec3dCurve2dInSurfaceOneD(dtCurve2d const * const dtC2d, dtSurface const * const dtS);
    virtual vec3dCurve2dInSurfaceOneD * clone( void ) const;
    virtual vec3dCurve2dInSurfaceOneD * create( void ) const;    
    virtual ~vec3dCurve2dInSurfaceOneD();
    virtual aFY Y( float const & xx) const;
    vectorHandling< renderInterface * > getRender( void ) const;
    //dtCurve const * ptrDtCurve( void ) const;
    virtual bool closed( int const & dir ) const;
  private:
    ptrHandling<dtCurve2d> _dtC2d;
    ptrHandling<dtSurface> _dtS;
    static int const _nVis = 100;
  };
}
#endif	/* VEC3DCURVE2DINSURFACEONED_H */

