#ifndef DTSURFACE2D_H
#define	DTSURFACE2D_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve2d;
  
  class dtSurface2d {
  public:
    dt__CLASSNAME(dtSurface2d);
    dtSurface2d();
    virtual ~dtSurface2d();
    //
    // overload
    //
    virtual dtSurface2d * clone( void ) const = 0;
    virtual float minPara( int const dim ) const = 0;
    virtual float maxPara( int const dim ) const = 0;
    virtual bool closed( int const dim ) const = 0; 
    virtual dtPoint2 point( float const uu, float const vv) const = 0;
    virtual std::vector<dtVector2> firstDer( float const uu, float const vv) const = 0;
    virtual std::vector<dtVector2> secondDer( float const uu, float const vv) const = 0;
	  virtual dtCurve2d * segmentConstU( float const uu, float const vvMin, float const vvMax) const = 0;
	  virtual dtCurve2d * segmentConstV( float const vv, float const uuMin, float const uuMax) const = 0;    
    virtual dtPoint2 reparam(dtPoint2 const point) const = 0;
    //
    // optional overload
    //
    virtual dtPoint2 controlPoint( int const uI, int const vI ) const;
    virtual void setControlPoint( int const uI, int const vI, dtPoint2 const point );    
    virtual int nControlPoints( int const dim ) const;    
    virtual void dump( void ) const ;
    //
    //
    //    
    float minU ( void ) const;
    float maxU ( void ) const;  
    float minV ( void ) const;
    float maxV ( void ) const;    
    bool closedU( void ) const;
    bool closedV( void ) const;
    float u_uPercent(float const percent) const;
    float uPercent_u(float const uu) const;
    float v_vPercent(float const percent) const;
    float vPercent_v(float const vv) const;  
    dtPoint2 point( dtPoint2 const & pUV) const;
    dtPoint2 pointPercent(float const uP, float const vP) const;   
    dtVector2 firstDerU( float const uu, float const vv) const;
    dtVector2 firstDerV( float const uu, float const vv) const ;
    dtVector2 secondDerUU( float const uu, float const vv) const;
    dtVector2 secondDerVV( float const uu, float const vv) const;
    dtVector2 secondDerUV( float const uu, float const vv) const;
    dtVector2 normalPercent( float const uP, float const vP ) const;
    dtPoint2 uv_uvPercent( dtPoint2 const uvPercent) const;  
    int nControlPointsU( void ) const;
    int nControlPointsV( void ) const;
    dtCurve2d * segmentConstU( float const uu) const;
    dtCurve2d * segmentConstV( float const vv) const;    
    dtCurve2d * segmentConstUPercent( float const uu) const;
    dtCurve2d * segmentConstVPercent( float const vv) const;    
    dtCurve2d * segmentConstUPercent( float const uu, float const vvMin, float const vvMax) const;
    dtCurve2d * segmentConstVPercent( float const vv, float const uuMin, float const uuMax) const;
    dtPoint2 reparamPercent(dtPoint2 const point) const;
  };
}
#endif	/* DTSURFACE2D_H */

