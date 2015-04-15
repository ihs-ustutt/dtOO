#ifndef SCAONEDINMAP1DTO3D_H
#define	SCAONEDINMAP1DTO3D_H

#include <dtLinearAlgebra.h>
#include "map1dTo3d.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class scaOneD;
  
  class scaOneDInMap1dTo3d : public map1dTo3d {
    public:
      dt__class(scaOneDInMap1dTo3d, analyticGeometry);
      scaOneDInMap1dTo3d();
      scaOneDInMap1dTo3d(const scaOneDInMap1dTo3d& orig);
      scaOneDInMap1dTo3d(scaOneD const * const s1d, map1dTo3d const * const m1d);
      scaOneDInMap1dTo3d(scaOneD const * const s1d, map1dTo3d const * const m1d, bool const & percentF);
      virtual ~scaOneDInMap1dTo3d();
      //
      // overload 
      //
      virtual scaOneDInMap1dTo3d * clone( void ) const;
      virtual scaOneDInMap1dTo3d * create( void ) const;    
      virtual dtPoint3 getPoint( float const & uu ) const;
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;
      //
      //
      //
      scaOneD const * ptrToScaOneD( void ) const;
      map1dTo3d const * ptrToMap1dTo3d( void ) const;      
  private:
    dt__pH(scaOneD) _s1d;
    dt__pH(map1dTo3d) _m1d;
    bool _percentF;
  };
}
#endif	/* SCAONEDINMAP1DTO3D_H */

