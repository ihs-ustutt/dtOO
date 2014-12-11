#ifndef DTSURFACE_H
#define	DTSURFACE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve;
  class dtCurve2d;
  
  class dtSurface {
  public:
    dt__CLASSNAME(dtSurface);
    dtSurface();
    virtual ~dtSurface();
    //
    // overload
    //
    virtual dtSurface * clone( void ) const = 0;
    virtual float minPara( int const dim ) const = 0;
    virtual float maxPara( int const dim ) const = 0;
    virtual bool closed( int const dim ) const = 0; 
    virtual dtPoint3 point( float const uu, float const vv) const = 0;
    virtual dtVector3 normal( float const uu, float const vv) const = 0;
    virtual std::vector<dtVector3> firstDer( float const uu, float const vv) const = 0;
    virtual std::vector<dtVector3> secondDer( float const uu, float const vv) const = 0;
	  virtual dtCurve * segmentConstU( float const uu, float const vvMin, float const vvMax) const = 0;
	  virtual dtCurve * segmentConstV( float const vv, float const uuMin, float const uuMax) const = 0;    
    virtual dtPoint2 reparam(dtPoint3 const point) const = 0;
    //
    // optional overload
    //
    virtual dtPoint3 controlPoint( int const uI, int const vI ) const;
    virtual void setControlPoint( int const uI, int const vI, dtPoint3 const point );    
    virtual int nControlPoints( int const dim ) const;    
    virtual void dump( void ) const ;
    virtual void offsetNormal(float const nn);        
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
    dtPoint3 getPoint3d( dtPoint2 const & pUV) const;
    dtPoint3 getPointPercent3d(float const uP, float const vP) const;   
    std::vector< dtPoint3 > 
    getPoint3d( 
      std::vector< float > const & uu, 
      std::vector< float > const & vv
    ) const;
    std::vector< dtPoint3 > 
    getPointPercent3d( 
      std::vector< float > const & uu, 
      std::vector< float > const & vv
    ) const;    
    std::vector< dtPoint3 > 
    getPoint3d( 
      std::vector< float > const * const uuV, 
      std::vector< float > const * const vvV
    ) const;
    dtVector3 firstDerU( float const uu, float const vv) const;
    dtVector3 firstDerV( float const uu, float const vv) const ;
    dtVector3 secondDerUU( float const uu, float const vv) const;
    dtVector3 secondDerVV( float const uu, float const vv) const;
    dtVector3 secondDerUV( float const uu, float const vv) const;
    dtVector3 normalPercent( float const uP, float const vP ) const;
    dtPoint2 uv_uvPercent( dtPoint2 const uvPercent) const;  
    int nControlPointsU( void ) const;
    int nControlPointsV( void ) const;
    dtCurve * segmentConstU( float const uu) const;
    dtCurve * segmentConstV( float const vv) const;    
    dtCurve * segmentConstUPercent( float const uu) const;
    dtCurve * segmentConstVPercent( float const vv) const;    
    dtCurve * segmentConstUPercent( float const uu, float const vvMin, float const vvMax) const;
    dtCurve * segmentConstVPercent( float const vv, float const uuMin, float const uuMax) const;
    dtPoint2 reparamPercent(dtPoint3 const point) const;
  };
}
#endif	/* DTSURFACE_H */

