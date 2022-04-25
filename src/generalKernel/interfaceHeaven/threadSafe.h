#ifndef THREADSAFE_H
#define	THREADSAFE_H

#include <dtOOTypeDef.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
  template< typename T >
  class threadSafe {
  public:
    dt__classOnlyName( threadSafe );
    threadSafe() : _t(staticPropertiesHandler::ompGetMaxThreads()) {
    }
    
    virtual ~threadSafe() {
      
    }
    
    T & operator()( void ) {
      return _t[ staticPropertiesHandler::ompGetThreadNum() ];
    };
  private:
    std::vector< T > _t;
  };
}

#endif	/* THREADSAFE_H */

