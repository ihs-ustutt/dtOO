#ifndef FLOATHANDLING_H
#define	FLOATHANDLING_H

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class floatHandling {
  public:
    dt__classOnlyName(floatHandling);
    virtual ~floatHandling();
    static float next(double a);
    static float prev(double a);
    static double boundToRange(float const value, double const a, double const b);
    static bool isAscending( std::vector< float > const & check);
    static bool isSmall( float const & val );
  private:
    floatHandling();
  };
}
#endif	/* FLOATHANDLING_H */

