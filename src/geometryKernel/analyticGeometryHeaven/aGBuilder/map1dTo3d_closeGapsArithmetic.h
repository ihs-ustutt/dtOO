#ifndef map1dTo3d_closeGapsArithmetic_H
#define	map1dTo3d_closeGapsArithmetic_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

namespace dtOO {
  class map1dTo3d;
  
  class map1dTo3d_closeGapsArithmetic {
    public:
      dt__classOnlyName(map1dTo3d_closeGapsArithmetic);        
      map1dTo3d_closeGapsArithmetic( dt__pVH(map1dTo3d)  const & m1d );
      virtual ~map1dTo3d_closeGapsArithmetic();
      dt__pVH(map1dTo3d) result( void );
    private:
      dt__pVH(map1dTo3d) _m1d;
  };
}
#endif	/* map1dTo3d_closeGapsArithmetic_H */

