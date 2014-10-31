#ifndef OPTIMIZABLEINTERFACE_H
#define	OPTIMIZABLEINTERFACE_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include "characterizable.h"

namespace dtOO {
  class constValue;
  
  class optimizableInterface : public characterizable {
  public:
    dt__CLASSNAME(optimizableInterface);    
    optimizableInterface();
    optimizableInterface(const optimizableInterface& orig);
    virtual ~optimizableInterface();
    virtual vectorHandling< constValue * > & getRefToParameters( void ) = 0;
    virtual int getNParameters( void ) const = 0;
  private:

  };
}
#endif	/* OPTIMIZABLEINTERFACE_H */

