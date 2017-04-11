#ifndef map3dTo3dGroup_H
#define	map3dTo3dGroup_H

#include "map3dTo3d.h"
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class dtTransformer;

  class map3dTo3dGroup : public map3dTo3d {
    public:
      dt__class(map3dTo3dGroup, analyticGeometry);    
      map3dTo3dGroup();
      map3dTo3dGroup( map3dTo3dGroup const & orig );
      map3dTo3dGroup( std::vector< map3dTo3d const * > const & vec );
      virtual ~map3dTo3dGroup();
      //
      // overload
      //      
      virtual map3dTo3dGroup * create( void ) const;
      virtual map3dTo3dGroup * clone( void ) const;
      virtual map3dTo3dGroup * cloneTransformed( dtTransformer const * const dtT ) const;
      virtual dtPoint3 getPoint(
        float const & uu, float const & vv, float const & ww 
      ) const;      
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;
      virtual vectorHandling< renderInterface * > getRender( void ) const;
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    private:
      dt__pVH(map3dTo3d) _vec;
  };
}
#endif	/* map3dTo3dGroup_H */

