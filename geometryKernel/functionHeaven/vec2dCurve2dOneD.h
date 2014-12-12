#ifndef VEC2DCURVE2DONED_H
#define	VEC2DCURVE2DONED_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "vec2dOneD.h"
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtCurve2d;
  class renderInterface;
  
  class vec2dCurve2dOneD : public vec2dOneD {
  public:
    dt__CLASSSTD(vec2dCurve2dOneD, analyticFunction);     
    vec2dCurve2dOneD();
    vec2dCurve2dOneD(const vec2dCurve2dOneD& orig);
    vec2dCurve2dOneD(dtCurve2d const * const dtC2d);
    virtual vec2dOneD * clone( void ) const;
    virtual vec2dOneD * create( void ) const;    
    virtual ~vec2dCurve2dOneD();
    virtual aFY Y( float const & xx) const;
    vectorHandling< renderInterface * > getRender( void ) const;
    dtCurve2d const * ptrDtCurve2d( void ) const;
    virtual bool closed( int const & dir ) const;
  private:
    ptrHandling<dtCurve2d> _dtC2d;
  };
}
#endif	/* VEC2DCURVE2DONED_H */

