#ifndef MAP2DTO3D_H
#define	MAP2DTO3D_H

#include "analyticGeometry.h"
#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>

namespace dtOO {
  class map1dTo3d;
  class dtTransformer;
  
  class map2dTo3d : public analyticGeometry {
  public:
    dt__CLASSSTD(map2dTo3d, analyticGeometry);
    map2dTo3d();
    map2dTo3d(const map2dTo3d& orig);
    virtual ~map2dTo3d();
    virtual dtPoint3 getPoint( float const & uu, float const & vv ) const = 0;
    dtPoint3 getPoint( dtPoint2 const & pUV ) const;
    dtPoint3 getPoint( dtPoint2 const * const pUV ) const;
    virtual std::vector< dtPoint3 > getPoint( std::vector< float > const & uu, std::vector< float > const & vv ) const;
    virtual std::vector< dtPoint3 > getPointPercent( std::vector< float > const & uu, std::vector< float > const & vv ) const;
    std::vector< dtVector3 > getPointPercentVector( std::vector< float > const & uu, std::vector< float > const & vv ) const;
    std::vector< std::vector< dtPoint3 > >  getPointGrid( int const & nU, int const & nV ) const;
    dtPoint3 getPointPercent( float const & uu, float const & vv ) const;
    dtVector3 getPointPercentVector( float const & uu, float const & vv ) const;
    virtual vectorHandling< renderInterface * > getRender( void ) const;
    virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    virtual bool isClosed( int const & dir) const = 0;
    virtual float getMin( int const & dir) const = 0;
    virtual float getMax( int const & dir) const = 0;
    virtual bool isClosedU( void ) const;
    virtual bool isClosedV( void ) const;
    virtual float getUMin( void ) const;
    virtual float getUMax( void ) const;
    virtual float getVMin( void ) const;
    virtual float getVMax( void ) const;     
    virtual dtVector3 normal( float const & uu, float const & vv) const;
    dtVector3 normalPercent( float const & uu, float const & vv ) const;    
    virtual dtVector3 firstDerU( float const & uu, float const & vv) const;
    virtual dtVector3 firstDerV( float const & uu, float const & vv) const;
    virtual dtVector3 secondDerUU( float const & uu, float const & vv) const;
    virtual dtVector3 secondDerVV( float const & uu, float const & vv) const;
    virtual dtVector3 secondDerUV( float const & uu, float const & vv) const; 
    virtual dtPoint2 reparamOnFace(dtPoint3 const & ppXYZ) const;
    dtPoint2 reparamPercentOnFace(dtPoint3 const & ppXYZ) const;
    int getRenderResolutionU( void ) const;
    int getRenderResolutionV( void ) const;        
    virtual map2dTo3d * clone( void ) const = 0;
    virtual map2dTo3d * cloneTransformed( dtTransformer const * const dtT ) const = 0;
    dtPoint2 uv_percent(dtPoint2 const & pUV) const;
    dtPoint2 uv_percent(float const & uu, float const & vv) const;
    float u_percent(float const & uu) const;
    float v_percent(float const & vv) const;
    dtPoint2 percent_uv(dtPoint2 const & pUV) const;
    float percent_u(float const & uu) const;
    float percent_v(float const & vv) const;
    virtual map1dTo3d * pickConstUPercent(
      float const & uu, float const & vv0, float const & vv1
    ) const = 0;
    virtual map1dTo3d * pickConstVPercent(
      float const & vv, float const & uu0, float const & uu1
    ) const = 0;
    virtual map1dTo3d * pickLinearUV(
      float const & uu0, float const & vv0, 
      float const & uu1, float const & vv1
    ) const = 0;
    map1dTo3d * pickLinearPercent(
      float const & uu0, float const & vv0, 
      float const & uu1, float const & vv1
    ) const;    
    virtual map2dTo3d * pickPercent(
      float const & uu0, float const & uu1, 
      float const & vv0, float const & vv1
    ) const = 0;
  private:
    bool XYZtoUV(double X, double Y, double Z, double &U, double &V,
                 double relax, std::vector< float > &itVal) const;
  private: 
  };
}
#endif	/* MAP2DTO3D_H */

