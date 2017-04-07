#ifndef map2dTo3dGroup_H
#define	map2dTo3dGroup_H

#include "map2dTo3d.h"
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class dtTransformer;

  class map2dTo3dGroup : public map2dTo3d {
    public:
      dt__class(map2dTo3dGroup, analyticGeometry);    
      map2dTo3dGroup();
      map2dTo3dGroup( map2dTo3dGroup const & orig );
      map2dTo3dGroup( std::vector< map2dTo3d const * > const & vec );
      virtual ~map2dTo3dGroup();
      //
      // overload
      //      
      virtual map2dTo3dGroup * create( void ) const;
      virtual map2dTo3dGroup * clone( void ) const;
      virtual map2dTo3dGroup * cloneTransformed( dtTransformer const * const dtT ) const;
      virtual dtPoint3 getPoint(float const & uu, float const & vv) const;      
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;
      virtual vectorHandling< renderInterface * > getRender( void ) const;
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    private:
      dt__pVH(map2dTo3d) _vec;
  };
}
#endif	/* map2dTo3dGroup_H */

