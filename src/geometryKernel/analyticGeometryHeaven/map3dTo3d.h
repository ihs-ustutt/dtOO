#ifndef MAP3DTO3D_H
#define	MAP3DTO3D_H

#include <dtLinearAlgebra.h>
#include "analyticGeometry.h"
#include <logMe/dtMacros.h>
#include <interfaceHeaven/twoDArrayHandling.h>

namespace dtOO {
  class map1dTo3d;
  class map2dTo3d;
  class dtTransformer;
  
  class map3dTo3d : public analyticGeometry {
  public:
    dt__class(map3dTo3d, analyticGeometry); 
    map3dTo3d();
    map3dTo3d(const map3dTo3d& orig);
    virtual ~map3dTo3d();
    virtual int dim( void ) const;   
    dtPoint3 getPoint( float const * const uvw ) const;
    virtual vectorHandling< renderInterface * > getRender( void ) const;
    //
    // overload
    //
    virtual map3dTo3d * create( void ) const = 0;
    virtual map3dTo3d * clone( void ) const = 0;
    virtual map3dTo3d * cloneTransformed(
      dtTransformer const * const dtT 
    ) const = 0;    
    virtual dtPoint3 getPoint( 
      float const & uu, float const & vv, float const & ww 
    ) const = 0;   
    //
    // optional overload
    //
    virtual dtPoint3 reparamInVolume(dtPoint3 const & ppXYZ) const;        
    virtual dtPoint3 approxInVolume(dtPoint3 const & ppXYZ) const; 
    virtual std::vector< dtVector3 > firstDer(
      float const & uu, float const & vv, float const & ww
    ) const;
    virtual map1dTo3d * segment(
      dtPoint3 const & p0, dtPoint3 const & p1
    ) const;
    virtual map2dTo3d * segment(
      twoDArrayHandling< dtPoint3 > const & pp 
    ) const;
    virtual map2dTo3d * segmentConstU( float const & uu ) const;
    virtual map2dTo3d * segmentConstV( float const & vv ) const;
    virtual map2dTo3d * segmentConstW( float const & ww ) const;
    virtual map2dTo3d * segmentConstU( 
      float const & uu, dtPoint2 const & p0, dtPoint2 const & p1 
    ) const;
    virtual map2dTo3d * segmentConstV( 
      float const & vv, dtPoint2 const & p0, dtPoint2 const & p1 
    ) const;
    virtual map2dTo3d * segmentConstW( 
      float const & ww, dtPoint2 const & p0, dtPoint2 const & p1 
    ) const;
    //
    //
    //
    dtVector3 firstDerU( 
      float const & uu, float const & vv, float const & ww 
    ) const;
    dtVector3 firstDerV( 
      float const & uu, float const & vv, float const & ww 
    ) const;    
    dtVector3 firstDerW( 
      float const & uu, float const & vv, float const & ww 
    ) const;
    dtPoint3 reparamPercentInVolume(dtPoint3 const & ppXYZ) const; 
    dtPoint3 approxPercentInVolume(dtPoint3 const & ppXYZ) const;     
    bool isClosedU( void ) const;
    bool isClosedV( void ) const;
    bool isClosedW( void ) const;
    float getUMin( void ) const;
    float getUMax( void ) const;
    float getVMin( void ) const;
    float getVMax( void ) const;     
    float getWMin( void ) const;
    float getWMax( void ) const;
    bool inRange( dtPoint3 const & pUVW ) const;
    dtPoint3 getPointPercent( 
      float const & uu, float const & vv, float const & ww 
    ) const;
    dtPoint3 getPoint( dtPoint3 const & ppUVW ) const;
    dtPoint3 getPointPercent( dtPoint3 const & ppUVW ) const;    
    float u_percent(float const & uu) const;
    float v_percent(float const & vv) const;
    float w_percent(float const & ww) const;    
    float percent_u(float const & uu) const;
    float percent_v(float const & vv) const;
    float percent_w(float const & ww) const;        
    dtPoint3 percent_uvw(dtPoint3 const & pUVW) const;
    dtPoint3 uvw_percent(dtPoint3 const & pp) const;    
    int getRenderResolutionU( void ) const;
    int getRenderResolutionV( void ) const;            
    int getRenderResolutionW( void ) const;
    map2dTo3d * segmentConstUPercent( 
      float const & uu, dtPoint2 const & p0, dtPoint2 const & p1 
    ) const;
    map2dTo3d * segmentConstVPercent( 
      float const & vv, dtPoint2 const & p0, dtPoint2 const & p1 
    ) const;
    map2dTo3d * segmentConstWPercent( 
      float const & ww, dtPoint2 const & p0, dtPoint2 const & p1 
    ) const;    
    map2dTo3d * segmentConstUPercent( float const & uu ) const;
    map2dTo3d * segmentConstVPercent( float const & vv ) const;
    map2dTo3d * segmentConstWPercent( float const & ww ) const;      
    map2dTo3d * segment( 
      dtPoint3 const & p0, dtPoint3 const & p1, 
      dtPoint3 const & p2, dtPoint3 const & p3 
    ) const;
    map2dTo3d * segmentPercent( 
      twoDArrayHandling< dtPoint3 > const & pp 
    ) const;
    map1dTo3d * segmentPercent( 
      dtPoint3 const & p0, dtPoint3 const & p1 
    ) const;
    map2dTo3d * segmentPercent( 
      dtPoint3 const & p0, dtPoint3 const & p1, 
      dtPoint3 const & p2, dtPoint3 const & p3 
    ) const;
  protected:
    dtPoint3 reparamInVolume(
      dtPoint3 const & ppXYZ, dtVector3 const & uvwExtPercent
    ) const;    
  private:
    bool XYZtoUVWPercent(
      double X, double Y, double Z, 
      double &U, double &V, double &W,
      double const uMin, double const uMax, 
      double const vMin, double const vMax, 
      double const wMin, double const wMax,     
      double const stepU, double const stepV, double const stepW, 
      double const prec    
    ) const;
	  double F(double const * xx) const;  
    double FWrap(
      double const & x0, double const & x1, double const & x2 
    ) const;
  private:
    mutable dtPoint3 _pXYZ;
    static float _deltaPer;
  };
}
#endif	/* MAP3DTO3D_H */

