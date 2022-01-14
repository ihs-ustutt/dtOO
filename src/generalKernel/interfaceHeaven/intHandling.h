#ifndef FLOATHANDLING_H
#define	FLOATHANDLING_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class intHandling {
  public:
    dt__classOnlyName(intHandling);
    virtual ~intHandling();
    static int round( dtReal a );
    static int bound( int const & toBound, int const & aa, int const & bb );
  private:
    intHandling();
  };
}
#endif	/* FLOATHANDLING_H */

