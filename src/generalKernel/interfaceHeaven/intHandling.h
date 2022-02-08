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
    static dtInt round( dtReal a );
    static dtInt bound( dtInt const & toBound, dtInt const & aa, dtInt const & bb );
  private:
    intHandling();
  };
}
#endif	/* FLOATHANDLING_H */

