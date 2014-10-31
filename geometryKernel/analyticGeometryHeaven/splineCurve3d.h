#ifndef SPLINECURVE3D_H
#define	SPLINECURVE3D_H

#include "map1dTo3d.h"
#include "oneDimRevertable.h"
#include "oneDimCloseable.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <deprecatedMacro.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;

  class splineCurve3d : public map1dTo3d,
                        public oneDimRevertable,
                        public oneDimCloseable {
  public:
    dt__CLASSSTD(splineCurve3d, analyticGeometry);
    splineCurve3d();
    splineCurve3d( splineCurve3d const & orig );
    splineCurve3d( dtCurve const & orig );
    splineCurve3d( dtCurve * orig );
    virtual splineCurve3d * clone( void ) const;
    virtual splineCurve3d * create( void ) const;       
    virtual ~splineCurve3d();
    virtual void dump(void) const;
    virtual dtPoint3 getPoint( float const & uu ) const;
    virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    dtCurve const * const ptrConstDtCurve(void) const;
    void rotate(dtPoint3 const origin, dtVector3 const vector, float const angle);
    virtual void revert(void);
    virtual void closeArithmetic( void );
    virtual void closeStraight( void );
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    virtual dtVector3 firstDerU( float const uu) const;    
  private:
    ptrHandling<dtCurve> _dtC;
  };
}
#endif	/* SPLINECURVE3D_H */

