#ifndef DTOMMESHDIVIDED_H
#define	DTOMMESHDIVIDED_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

#include "dtOMMesh.h"

class MElement;
class MVertex;

namespace dtOO {
  class dtOMMeshDivided {
  public:
    dt__classOnlyName(dtOMMeshDivided);    
    dtOMMeshDivided();
    virtual ~dtOMMeshDivided();
    void add( dtOMMesh const & om );
    dtOMMesh connect( void );
  private:
    std::list< dtOMMesh > _part;
  };
}
#endif	/* DTOMMESHDIVIDED_H */

