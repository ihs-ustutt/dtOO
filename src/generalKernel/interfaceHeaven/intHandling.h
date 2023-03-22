#ifndef intHandling_H
#define	intHandling_H

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
#endif	/* intHandling_H */

