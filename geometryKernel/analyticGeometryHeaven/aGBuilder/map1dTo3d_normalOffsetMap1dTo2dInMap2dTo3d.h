#ifndef map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d_H
#define	map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class map1dTo3d;
  
  class map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d {
    public:
      dt__CLASSNAME(map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d);        
      map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d(
        map1dTo3d const * const m1d, float const & thick
      );
      virtual ~map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d();
      map1dTo3d * result( void );
    private:
      dt__pH(map1dTo3d) _m1d;
  };
}
#endif	/* map1dTo3d_normalOffsetMap1dTo2dInMap2dTo3d_H */
