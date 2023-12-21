#ifndef DTSURFACE2D_H
#define	DTSURFACE2D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtCurve2d;
  
  class dtSurface2d {
  public:
    dt__classOnlyName(dtSurface2d);
    dtSurface2d();
    virtual ~dtSurface2d();
    //
    // overload
    //
    virtual dtSurface2d * clone( void ) const = 0;
    virtual dtReal minPara( dtInt const dim ) const = 0;
    virtual dtReal maxPara( dtInt const dim ) const = 0;
    virtual bool closed( dtInt const dim ) const = 0; 
    virtual dtPoint2 point( dtReal const uu, dtReal const vv) const = 0;
    virtual std::vector<dtVector2> firstDer( 
      dtReal const uu, dtReal const vv
    ) const = 0;
    virtual std::vector<dtVector2> secondDer( 
      dtReal const uu, dtReal const vv
    ) const = 0;
	  virtual dtCurve2d * segmentConstU( 
      dtReal const uu, dtReal const vvMin, dtReal const vvMax
    ) const = 0;
	  virtual dtCurve2d * segmentConstV( 
      dtReal const vv, dtReal const uuMin, dtReal const uuMax
    ) const = 0;    
    virtual dtPoint2 reparam(dtPoint2 const point) const = 0;
    //
    // optional overload
    //
    virtual dtPoint2 controlPoint( dtInt const uI, dtInt const vI ) const;
    virtual void setControlPoint( 
      dtInt const uI, dtInt const vI, dtPoint2 const point 
    );    
    virtual dtInt nControlPoints( dtInt const dim ) const;    
    void dump( void ) const;
    virtual std::string dumpToString(void) const;
    //
    //
    //    
    dtReal minU ( void ) const;
    dtReal maxU ( void ) const;  
    dtReal minV ( void ) const;
    dtReal maxV ( void ) const;    
    bool closedU( void ) const;
    bool closedV( void ) const;
    dtReal u_uPercent(dtReal const percent) const;
    dtReal uPercent_u(dtReal const uu) const;
    dtReal v_vPercent(dtReal const percent) const;
    dtReal vPercent_v(dtReal const vv) const;  
    dtPoint2 point( dtPoint2 const & pUV) const;
    dtPoint2 pointPercent(dtReal const uP, dtReal const vP) const;   
    dtVector2 firstDerU( dtReal const uu, dtReal const vv) const;
    dtVector2 firstDerV( dtReal const uu, dtReal const vv) const ;
    dtVector2 secondDerUU( dtReal const uu, dtReal const vv) const;
    dtVector2 secondDerVV( dtReal const uu, dtReal const vv) const;
    dtVector2 secondDerUV( dtReal const uu, dtReal const vv) const;
    dtPoint2 uv_uvPercent( dtPoint2 const uvPercent) const;  
    dtInt nControlPointsU( void ) const;
    dtInt nControlPointsV( void ) const;
    dtCurve2d * segmentConstU( dtReal const uu) const;
    dtCurve2d * segmentConstV( dtReal const vv) const;    
    dtCurve2d * segmentConstUPercent( dtReal const uu) const;
    dtCurve2d * segmentConstVPercent( dtReal const vv) const;    
    dtCurve2d * segmentConstUPercent( 
      dtReal const uu, dtReal const vvMin, dtReal const vvMax
    ) const;
    dtCurve2d * segmentConstVPercent( 
      dtReal const vv, dtReal const uuMin, dtReal const uuMax
    ) const;
    dtPoint2 reparamPercent(dtPoint2 const point) const;
  };
}
#endif	/* DTSURFACE2D_H */

