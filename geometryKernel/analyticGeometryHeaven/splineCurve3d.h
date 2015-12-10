#ifndef SPLINECURVE3D_H
#define	SPLINECURVE3D_H

#include "map1dTo3d.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <deprecatedMacro.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;

  class splineCurve3d : public map1dTo3d {
    public:
      dt__class(splineCurve3d, analyticGeometry);
      splineCurve3d();
      splineCurve3d( splineCurve3d const & orig );
      splineCurve3d( dtCurve const & orig );
      splineCurve3d( dtCurve * orig );
      virtual ~splineCurve3d();
      //
      // overload
      //
      virtual splineCurve3d * create( void ) const;       
      virtual splineCurve3d * clone( void ) const;
      virtual splineCurve3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const;       
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;        
      virtual dtPoint3 getPoint( float const & uu ) const;
      //
      // optional overload
      //
      virtual dtVector3 firstDerU( float const uu) const;
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
      dtCurve const * const ptrConstDtCurve(void) const;
    private:
      dt__pH(dtCurve) _dtC;
  };
}
#endif	/* SPLINECURVE3D_H */

