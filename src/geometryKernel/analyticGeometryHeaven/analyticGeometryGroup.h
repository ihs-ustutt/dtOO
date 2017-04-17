#ifndef analyticGeometryGroup_H
#define	analyticGeometryGroup_H

#include "analyticGeometry.h"
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class dtTransformer;

  class analyticGeometryGroup : public analyticGeometry {
    public:
      dt__class(analyticGeometryGroup, analyticGeometry);    
      analyticGeometryGroup();
      analyticGeometryGroup( analyticGeometryGroup const & orig );
      analyticGeometryGroup( std::vector< analyticGeometry const * > const & vec );
      virtual ~analyticGeometryGroup();
      //
      // overload
      //      
      virtual analyticGeometryGroup * clone( void ) const;      
      virtual analyticGeometryGroup * create( void ) const;
      virtual int dim( void ) const;      
      virtual bool isClosed( int const & dir) const;
      virtual float getMin( int const & dir) const;
      virtual float getMax( int const & dir) const;
      virtual dtPoint3 getPoint( float const * const uvw ) const;      
      virtual vectorHandling< renderInterface * > getRender( void ) const;
      virtual vectorHandling< renderInterface * > getExtRender( void ) const;
    private:
      dt__pVH(analyticGeometry) _vec;
  };
}
#endif	/* analyticGeometryGroup_H */

