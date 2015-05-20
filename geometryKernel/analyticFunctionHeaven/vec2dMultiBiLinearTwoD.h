#ifndef vec2dMultiBiLinearTwoD_H
#define	vec2dMultiBiLinearTwoD_H

#include "vec2dTwoD.h"
#include "vec2dBiLinearTwoD.h"
#include <interfaceHeaven/twoDArrayHandling.h>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class vec2dMultiBiLinearTwoD : public vec2dTwoD {
  public:
    dt__class(vec2dMultiBiLinearTwoD, analyticFunction);     
    vec2dMultiBiLinearTwoD();
    vec2dMultiBiLinearTwoD( vec2dMultiBiLinearTwoD const & orig );
    vec2dMultiBiLinearTwoD( twoDArrayHandling< dtPoint2 > const & pp );
    vec2dMultiBiLinearTwoD * clone( void ) const;
    vec2dMultiBiLinearTwoD * create( void ) const;    
    virtual ~vec2dMultiBiLinearTwoD();
    virtual aFY Y( aFX const & xx ) const;
    virtual bool closed( int const & dir ) const;    
    aFX invY(aFY const & yy) const;
  private:
    twoDArrayHandling< vec2dBiLinearTwoD > _piece;
  };
}
#endif	/* vec2dMultiBiLinearTwoD_H */