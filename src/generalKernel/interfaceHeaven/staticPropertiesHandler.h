#ifndef staticPropertiesHandler_H
#define	staticPropertiesHandler_H

#include <dtOOTypeDef.h>

#include "optionHandling.h"
#include <logMe/dtMacros.h>
#include <jsonHeaven/jsonPrimitive.h>

namespace dtOO {
  class staticPropertiesHandler : public optionHandling {
    public:
      dt__classOnlyName(staticPropertiesHandler);
      static staticPropertiesHandler * getInstance( void );
      virtual ~staticPropertiesHandler();
      virtual void jInit(jsonPrimitive const & jE);
      virtual void init(::QDomElement const * const wElement);
      static bool mpiParallel( void );
      dtInt thisRank( void ) const; 
      dtInt nRanks( void ) const;
      bool initialized( void ) const;
      static void mpiBarrier( void );
      static dtInt ompGetMaxThreads( void );
      static dtInt ompGetThreadNum( void );
      static bool ompInParallel( void );
      static dtInt ompGetNumThreads( void );
      static dtInt ompGetThreadLimit( void );
      bool gDebug( void ) const;
      void setGDebug( void );
    protected:
      jsonPrimitive & config( void );
      jsonPrimitive const & config( void ) const;
    private:
      staticPropertiesHandler();
    private:
      static dt__pH(staticPropertiesHandler) _pH;
      dtInt _thisRank;
      dtInt _nRanks;
      bool _initialized;
      bool _gDebug;
      jsonPrimitive _config;
  };
}
#endif	/* staticPropertiesHandler_H */

