#ifndef THREADSAFE_H
#define	THREADSAFE_H

#include <dtOOTypeDef.h>

#include <omp.h>

namespace dtOO {
  template< typename T >
  class threadSafe {
  public:
    dt__classOnlyName( mpiDistributed );
    threadSafe() : _t(omp_get_max_threads()) {
    }
    
    virtual ~threadSafe() {
      
    }
    
    T & operator()( void ) {
      return _t[ omp_get_thread_num() ];
    };
  private:
    std::vector< T > _t;
  };
}

#endif	/* THREADSAFE_H */

