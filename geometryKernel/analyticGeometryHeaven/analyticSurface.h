#ifndef ANALYTICSURFACE_H
#define	ANALYTICSURFACE_H

#define _USE_MATH_DEFINES 
#include <math.h>
#include "map2dTo3d.h"
#include <dtLinearAlgebra.h>
#include <vector>
#include "twoDimRevertable.h"

namespace dtOO {
  class dtSurface;
  class dtCurve;
  class dtCurve2d;
  class splineCurve3d;

  /**
   * Analytic surface.
   * @author A. Tismer
   * @date 04.12.2012
   */
  class analyticSurface : public map2dTo3d,
                          public twoDimRevertable {
  public:
    dt__CLASSSTD(analyticSurface, analyticGeometry);    
    //
    // construction and destruction
    //
    analyticSurface();
    analyticSurface( analyticSurface const & orig );
    analyticSurface(dtSurface const * const surface);
    virtual analyticSurface * clone( void ) const;
    virtual analyticSurface * create( void ) const;
    virtual ~analyticSurface();
    //
    // read and modify member attributes
    //
    dtSurface * ptrDtSurface( void ) const;
    dtSurface const * constPtrDtSurface( void ) const; 
    //
    // output and visualization
    //
    virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    //
    // point picking and normal vectors
    //
    virtual dtPoint3 getPoint(float const & uu, float const & vv) const;
    //
    // interfaces
    //
    virtual void revert( void );
    virtual void offsetNormal( float const nn );
    virtual bool isClosed( int const & dir) const;
    virtual float getMin( int const & dir) const;
    virtual float getMax( int const & dir) const;
    virtual dtVector3 normal( float const & uu, float const & vv) const;
    virtual dtVector3 firstDerU( float const & uu, float const & vv) const;
    virtual dtVector3 firstDerV( float const & uu, float const & vv) const;
    virtual dtVector3 secondDerUU( float const & uu, float const & vv) const;
    virtual dtVector3 secondDerVV( float const & uu, float const & vv) const;
    virtual dtVector3 secondDerUV( float const & uu, float const & vv) const;
    virtual dtPoint2 reparamOnFace(dtPoint3 const & ppXYZ) const;
    virtual map1dTo3d * pickConstUPercent(
      float const & uu, float const & vv0, float const & vv1
    ) const;
    virtual map1dTo3d * pickConstVPercent(
      float const & vv, float const & uu0, float const & uu1
    ) const;
    map1dTo3d * pickLinearUV( 
      float const & uu0, float const & vv0,
      float const & uu1, float const & vv1 
    ) const;
    virtual map2dTo3d * pickPercent(
      float const & uu0, float const & uu1, 
      float const & vv0, float const & vv1
    ) const;    
  private:
    dt__pH(dtSurface) _dtS;
  };
}
#endif	/* ANALYTICSURFACE_H */

