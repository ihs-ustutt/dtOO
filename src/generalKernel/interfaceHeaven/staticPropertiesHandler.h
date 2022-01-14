#ifndef staticPropertiesHandler_H
#define	staticPropertiesHandler_H

#include <dtOOTypeDef.h>

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
    bool initialized( void ) const;
    static void mpiBarrier( void );
    bool gDebug( void ) const;
    void setGDebug( void );
  private:
    staticPropertiesHandler();
  private:
    static dt__pH(staticPropertiesHandler) _pH;
    int _thisRank;
    int _nRanks;
    bool _initialized;
    bool _gDebug;
  };
}
#endif	/* staticPropertiesHandler_H */

