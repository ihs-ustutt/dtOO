#ifndef ANALYTICSURFACE_H
#define	ANALYTICSURFACE_H

#include <dtOOTypeDef.h>

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
      virtual dtPoint3 getPoint(dtReal const & uu, dtReal const & vv) const;      
      virtual bool isClosed( int const & dir) const;
      virtual dtReal getMin( int const & dir) const;
      virtual dtReal getMax( int const & dir) const;
      //
      // optional overload
      //
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
      virtual dtVector3 normal( dtReal const & uu, dtReal const & vv) const;
      virtual std::vector< dtVector3 > firstDer( 
        dtReal const & uu, dtReal const & vv
      ) const;
      virtual std::vector< dtVector3 > secondDer( 
        dtReal const & uu, dtReal const & vv
      ) const;
      virtual dtPoint2 reparamOnFace(dtPoint3 const & ppXYZ) const;
      virtual map1dTo3d * segmentConstU(
        dtReal const & uu, dtReal const & p0, dtReal const & p1
      ) const;
      virtual map1dTo3d * segmentConstV(
        dtReal const & vv, dtReal const & p0, dtReal const & p1
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

      virtual void offsetNormal( dtReal const nn );
    private:
      dt__pH(dtSurface) _dtS;
  };
}
#endif	/* ANALYTICSURFACE_H */

