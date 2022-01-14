#ifndef DTOMFIELD_H
#define	DTOMFIELD_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/labelHandling.h>
#include "dtOMMesh.h"

namespace dtOO {
  class dtOMField : public labelHandling {
  public:
    dt__classOnlyName(dtOMField);
    dtOMField( std::string const & label, dtOMMesh const & om );
    virtual ~dtOMField();
    dtOMMesh const & refMesh( void ) const;
    virtual void update( void ) = 0;
  private:
    dtOMMesh const & _om;
  };
}
#endif	/* DTOMFIELD_H */

