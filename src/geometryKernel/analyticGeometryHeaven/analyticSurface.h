#ifndef ANALYTICSURFACE_H
#define	ANALYTICSURFACE_H

#include "map2dTo3d.h"
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class dtSurface;
  class dtTransformer;

  class analyticSurface : public map2dTo3d {
    public:
      dt__class(analyticSurface, analyticGeometry);    
      analyticSurface();
      analyticSurface( analyticSurface const & orig );
      analyticSurface(dtSurface const * const surface);
      virtual ~analyticSurface();
      //
      // overload
      //      
      virtual analyticSurface * create( void ) const;
      virtual analyticSurface * clone( void ) const;
      virtual analyticSurface * 
      cloneTransformed( dtTransformer const * const dtT ) const;
      virtual dtPoint3 getPoint(float const & uu, float const & vv) const;      
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;
      //
      // optional overload
      //
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
      virtual dtVector3 normal( float const & uu, float const & vv) const;
      virtual std::vector< dtVector3 > firstDer( 
        float const & uu, float const & vv
      ) const;
      virtual std::vector< dtVector3 > secondDer( 
        float const & uu, float const & vv
      ) const;
      virtual dtPoint2 reparamOnFace(dtPoint3 const & ppXYZ) const;
      virtual map1dTo3d * segmentConstU(
        float const & uu, float const & p0, float const & p1
      ) const;
      virtual map1dTo3d * segmentConstV(
        float const & vv, float const & p0, float const & p1
      ) const;
      virtual map2dTo3d * segmentRectangle(
        dtPoint2 const & p0, dtPoint2 const & p1
      ) const;          
      virtual std::string dumpToString( void ) const;      
      //
      // read and modify member attributes
      //
      dtSurface * ptrDtSurface( void ) const;
      dtSurface const * ptrConstDtSurface( void ) const; 

      virtual void offsetNormal( float const nn );
    private:
      dt__pH(dtSurface) _dtS;
  };
}
#endif	/* ANALYTICSURFACE_H */

