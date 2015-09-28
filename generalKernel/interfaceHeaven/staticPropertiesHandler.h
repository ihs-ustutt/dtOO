#ifndef staticPropertiesHandler_H
#define	staticPropertiesHandler_H

#include "optionHandling.h"
#include <logMe/dtMacros.h>

namespace dtOO {
  class staticPropertiesHandler : public optionHandling {
  public:
    dt__classOnlyName(staticPropertiesHandler);
    static staticPropertiesHandler * getInstance( void );
    virtual ~staticPropertiesHandler();
    virtual void init(::QDomElement const * const wElement);    
  private:
    staticPropertiesHandler();
  private:
    static staticPropertiesHandler * _pH;
  };
}
#endif	/* staticPropertiesHandler_H */

