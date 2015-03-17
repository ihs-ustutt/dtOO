#ifndef DTOMMESHMANIFOLD_H
#define	DTOMMESHMANIFOLD_H

#include <logMe/dtMacros.h>
#include "dtOMMesh.h"

namespace dtOO {
  class dtOMMeshManifold : public dtOMMesh {
  public:
    dt__CLASSNAME(dtOMMeshManifold);
    dtOMMeshManifold(dtOMMesh const & om, omVertexH const & vH);
    virtual ~dtOMMeshManifold();
  private:

  };
}
#endif	/* DTOMMESHMANIFOLD_H */

