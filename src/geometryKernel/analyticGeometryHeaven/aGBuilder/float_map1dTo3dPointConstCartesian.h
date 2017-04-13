#ifndef float_map1dTo3dPointConstCartesian_H
#define	float_map1dTo3dPointConstCartesian_H

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class map1dTo3d;
  
  class float_map1dTo3dPointConstCartesian {
  public:
    dt__classOnlyName(float_map1dTo3dPointConstCartesian);     
    float_map1dTo3dPointConstCartesian(
      map1dTo3d const * const m1d, int const & dir, float const & value
    );
    virtual ~float_map1dTo3dPointConstCartesian();
    float result( void );
  private:
    double F(double const * xx) const;
  private:
    float _u;
    int _dir;
    float _value;
    map1dTo3d const & _m1d;    
  };
}
#endif	/* float_map1dTo3dPointConstCartesian_H */

