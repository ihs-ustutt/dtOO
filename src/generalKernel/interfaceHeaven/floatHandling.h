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
    static float nextIfSmaller(double a);
    static float prev(double a);
    static float prevIfBigger(double a);
    static float boundToRange(float const value, float const a, float const b);
    static bool isAscending( std::vector< float > const & check);
    static bool isSmall( float const & val );
  private:
    floatHandling();
  };
}
#endif	/* FLOATHANDLING_H */

