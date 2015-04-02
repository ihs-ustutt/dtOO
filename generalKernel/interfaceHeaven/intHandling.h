#ifndef FLOATHANDLING_H
#define	FLOATHANDLING_H

#include <logMe/dtMacros.h>
#include <vector>

namespace dtOO {
  class intHandling {
  public:
    dt__classOnlyName(intHandling);
    virtual ~intHandling();
    static int round( float a );
  private:
    intHandling();
  };
}
#endif	/* FLOATHANDLING_H */

