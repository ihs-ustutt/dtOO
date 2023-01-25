#ifndef float_map1dTo3dPointConstCartesian_H
#define	float_map1dTo3dPointConstCartesian_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map1dTo3d;
  
  class float_map1dTo3dPointConstCartesian {
    public:
      dt__classOnlyName(float_map1dTo3dPointConstCartesian);     
      float_map1dTo3dPointConstCartesian(
        map1dTo3d const * const m1d, dtInt const & dir, dtReal const & value,
        dtReal const & initGuess
      );        
      float_map1dTo3dPointConstCartesian(
        map1dTo3d const * const m1d, dtInt const & dir, dtReal const & value
      );
      virtual ~float_map1dTo3dPointConstCartesian();
      dtReal result( void );
    private:
      double F(double const * xx) const;
    private:
      dtReal _u;
      dtInt _dir;
      dtReal _value;
      map1dTo3d const & _m1d;    
  };
}
#endif	/* float_map1dTo3dPointConstCartesian_H */

