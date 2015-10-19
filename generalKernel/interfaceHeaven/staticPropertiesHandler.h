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
    static bool mpiParallel( void );
    int thisRank( void ) const; 
    int nRanks( void ) const;
  private:
    staticPropertiesHandler();
  private:
    static staticPropertiesHandler * _pH;
    int _thisRank;
    int _nRanks;
  };
}
#endif	/* staticPropertiesHandler_H */

