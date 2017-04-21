#ifndef analyticCurve_H
#define	analyticCurve_H

#include "map1dTo3d.h"
#include <vector>
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <deprecatedMacro.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;

  class analyticCurve : public map1dTo3d {
    public:
      dt__class(analyticCurve, analyticGeometry);
      analyticCurve();
      analyticCurve( analyticCurve const & orig );
      analyticCurve( dtCurve const & orig );
      analyticCurve( dtCurve * orig );
      virtual ~analyticCurve();
      //
      // overload
      //
      virtual analyticCurve * create( void ) const;       
      virtual analyticCurve * clone( void ) const;
      virtual analyticCurve * cloneTransformed( 
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
      virtual map1dTo3d * segment( float const & u0, float const & u1 ) const;  
      virtual float l_u( float const & uu ) const;
      virtual float u_l( float const & ll ) const;      
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
      dtCurve * ptrDtCurve(void) const;
      dtCurve const * const ptrConstDtCurve(void) const;
	    std::string dumpToString( void ) const;
    private:
      dt__pH(dtCurve) _dtC;
  };
}
#endif	/* analyticCurve_H */

