#ifndef SCAPIECEWISELINEAR_H
#define	SCAPIECEWISELINEAR_H

#include <logMe/dtMacros.h>

#include "scaFunction.h"
#include <dtLinearAlgebra.h>

namespace covise {
  class coDoSet;
  class coDoVec2;  
}

namespace dtOO {
  class scaPiecewiseLinear : public scaFunction {
  public:
    dt__classOnlyName(scaPiecewiseLinear);
    scaPiecewiseLinear();
    scaPiecewiseLinear(std::vector< dtPoint2> const * point);
    scaPiecewiseLinear(std::vector< dtPoint2> const & point);
    scaPiecewiseLinear(const scaPiecewiseLinear& orig);
    scaPiecewiseLinear * clone( void ) const;
    scaPiecewiseLinear * create( void ) const;
    virtual ~scaPiecewiseLinear();
    virtual void dump(void);
    virtual float getValue(float const xx) const;
    virtual std::vector< float > getVectorValue(float const xx) const;
    virtual covise::coDoVec2 * toCoDoVec2(char const * const str);
    virtual covise::coDoSet * toCoDoSet(char const * const str) const;
    virtual covise::coDoSet * packToCoDoSet( char const * const str) const;
    virtual void unpackFromCoDoSet( covise::coDoSet const * const set );         
  private:
    std::vector< dtPoint2 > _point;

  };
}
#endif	/* SCAPIECEWISELINEAR_H */

