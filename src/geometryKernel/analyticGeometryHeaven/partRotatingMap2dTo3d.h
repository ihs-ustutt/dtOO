#ifndef partRotatingMap2dTo3d_H
#define	partRotatingMap2dTo3d_H

#include "rotatingMap2dTo3d.h"

namespace dtOO {
  class partRotatingMap2dTo3d : public rotatingMap2dTo3d {
    public:
      dt__class(partRotatingMap2dTo3d, analyticGeometry);
      partRotatingMap2dTo3d();
      partRotatingMap2dTo3d( 
        dtVector3 const & vv, map2dTo3d const * const m2d ,
        dtReal const & minB, dtReal const & maxB
      );
      partRotatingMap2dTo3d(const partRotatingMap2dTo3d& orig);
      virtual ~partRotatingMap2dTo3d();
      virtual partRotatingMap2dTo3d * create( void ) const;        
      virtual partRotatingMap2dTo3d * clone( void ) const;
      virtual partRotatingMap2dTo3d * cloneTransformed( 
        dtTransformer const * const dtT 
      ) const;       
      virtual bool isClosed( dtInt const & dir) const;
      virtual dtReal getMin( dtInt const & dir) const;
      virtual dtReal getMax( dtInt const & dir) const;
      virtual dtPoint3 getPoint( 
        dtReal const & uu, dtReal const & vv, dtReal const & ww 
      ) const; 
	    virtual map2dTo3d * segmentConstV( dtReal const & ww ) const;
	    virtual map2dTo3d * segmentConstW( dtReal const & ww ) const;
    private:
      dtReal _minB;
      dtReal _maxB;
  };
}
#endif	/* partRotatingMap2dTo3d_H */

