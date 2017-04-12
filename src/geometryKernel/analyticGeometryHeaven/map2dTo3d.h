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
      dt__class(map2dTo3d, analyticGeometry);
      map2dTo3d();
      map2dTo3d(const map2dTo3d& orig);
      virtual ~map2dTo3d();
      virtual int dim( void ) const;                  
      virtual dtPoint3 getPoint( float const * const uvw ) const;      
      //
      // overload
      //
      virtual map2dTo3d * create( void ) const = 0;
      virtual map2dTo3d * clone( void ) const = 0;
      virtual map2dTo3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const = 0;
      virtual dtPoint3 getPoint( float const & uu, float const & vv ) const = 0;
      //
      // optional overload
      //
      virtual vectorHandling< renderInterface * > getRender( void ) const;
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
      virtual std::vector< dtVector3 > firstDer( dtPoint2 const & pp) const;      
      virtual std::vector< dtVector3 > secondDer( 
        float const & uu, float const & vv
      ) const;            
      virtual dtVector3 normal( float const & uu, float const & vv) const; 
      virtual dtPoint2 reparamOnFace(dtPoint3 const & ppXYZ) const;
      virtual dtPoint2 approxOnFace(dtPoint3 const & ppXYZ) const;
      virtual map1dTo3d * segment( 
        dtPoint2 const & p0, dtPoint2 const & p1 
      ) const;
      virtual map1dTo3d * segmentConstU( 
        float const & uu, float const & p0, float const & p1 
      ) const;
      virtual map1dTo3d * segmentConstV( 
        float const & vv, float const & p0, float const & p1 
      ) const;
      virtual map2dTo3d * segment( 
        dtPoint2 const & p0, dtPoint2 const & p1, 
        dtPoint2 const & p2, dtPoint2 const & p3 
      ) const;
      virtual map2dTo3d * segmentRectangle(
        dtPoint2 const & p0, dtPoint2 const & p1 
      ) const;
      //
      //
      //    
      dtPoint3 getPoint( dtPoint2 const & pUV ) const;
      dtPoint3 getPoint( dtPoint2 const * const pUV ) const;
      dtPoint3 getPointPercent( float const & uu, float const & vv ) const;
      dtPoint3 getPointPercent( dtPoint2 const & pUV ) const;
      dtVector3 getPointPercentVector( 
        float const & uu, float const & vv 
      ) const;
      dtMatrix jacobi( float const & uu, float const & vv) const;
      dtMatrix jacobi( dtPoint2 const & pp ) const;        
      bool isClosedU( void ) const;
      bool isClosedV( void ) const;
      float getUMin( void ) const;
      float getUMax( void ) const;
      float getVMin( void ) const;
      float getVMax( void ) const;
      bool inRange( dtPoint2 const & pUV ) const;
      dtVector3 normal( dtPoint2 const & pp ) const;      
      dtVector3 normalPercent( float const & uu, float const & vv ) const;   
      std::vector< dtVector3 > firstDer( 
        float const & uu, float const & vv
      ) const;         
      dtVector3 firstDerU( float const & uu, float const & vv) const;
      dtVector3 firstDerV( float const & uu, float const & vv) const;
      dtVector3 secondDerUU( float const & uu, float const & vv) const;
      dtVector3 secondDerVV( float const & uu, float const & vv) const;
      dtVector3 secondDerUV( float const & uu, float const & vv) const;           
      dtPoint2 reparamPercentOnFace(dtPoint3 const & ppXYZ) const;
      dtPoint2 approxPercentOnFace(dtPoint3 const & ppXYZ) const;      
      int getRenderResolutionU( void ) const;
      int getRenderResolutionV( void ) const;        
      dtPoint2 uv_percent(dtPoint2 const & pUV) const;
      dtPoint2 uv_percent(float const & uu, float const & vv) const;
      float u_percent(float const & uu) const;
      float v_percent(float const & vv) const;
      dtPoint2 percent_uv(dtPoint2 const & pUV) const;
      float percent_u(float const & uu) const;
      float percent_v(float const & vv) const;
      map1dTo3d * segmentConstU( float const & uu ) const;
      map1dTo3d * segmentConstV( float const & vv ) const;    
      map1dTo3d * segmentConstUPercent( 
        float const & uu, float const & p0, float const & p1 
      ) const;
      map1dTo3d * segmentConstVPercent( 
        float const & vv, float const & p0, float const & p1 
      ) const;
      map1dTo3d * segmentConstUPercent( float const & uu ) const;
      map1dTo3d * segmentConstVPercent( float const & vv ) const;
      map1dTo3d * segmentPercent( 
        dtPoint2 const & p0, dtPoint2 const & p1 
      ) const;    
      map2dTo3d * segmentPercent( 
        dtPoint2 const & p0, dtPoint2 const & p1, 
        dtPoint2 const & p2, dtPoint2 const & p3 
      ) const;    
      map2dTo3d * segmentRectanglePercent( 
        dtPoint2 const & p0, dtPoint2 const & p1 
      ) const;
      dtPoint2 operator%(const dtPoint2 &percent) const;      
    private:
//      bool XYZtoUVPercentGmsh(
//        double X, double Y, double Z, double &U, double &V, 
//        double const uMin, double const uMax, 
//        double const vMin, double const vMax, 
//        double const stepU, double const stepV, double const prec
//      ) const;
      bool XYZtoUVPercent(
        double X, double Y, double Z, double &U, double &V, 
        double const uMin, double const uMax, 
        double const vMin, double const vMax, 
        double const stepU, double const stepV, double const prec
      ) const;      
	    double F(double const * xx) const;        
	    double FWrap(double const & x0, double const & x1) const;
    private:
      mutable dtPoint3 _pXYZ;      
      static float _deltaPer;      
  };
  
	//
	// boost requires this method in ptr_vector
	//    
  map2dTo3d * new_clone(map2dTo3d const & m2d);
}
#endif	/* MAP2DTO3D_H */

