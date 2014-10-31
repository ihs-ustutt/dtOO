#ifndef SPLINESURFACE3D_H
#define	SPLINESURFACE3D_H

#include "analyticSurface.h"
#include "map1dTo3d.h"
#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  class map1dTo3d;

  /**
   * Spline surface class
   * @author M. Schlipf
   * @date 30.11.2012
   */
  class splineSurface3d : public analyticSurface {
  public:  
    dt__CLASSSTD(splineSurface3d, analyticGeometry);   
    splineSurface3d();
    splineSurface3d(dtSurface const * const surface);
    splineSurface3d( splineSurface3d const & orig );
    virtual ~splineSurface3d();
    virtual splineSurface3d * clone( void ) const;
    virtual splineSurface3d * create( void ) const;    
  private:
  };
}
#endif	/* SPLINESURFACE3D_H */

