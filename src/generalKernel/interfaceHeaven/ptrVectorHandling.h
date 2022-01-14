#ifndef PTRVECTORHANDLING_H
#define	PTRVECTORHANDLING_H

#include <dtOOTypeDef.h>

#include <boost/ptr_container/ptr_vector.hpp>

namespace dtOO {
  template < typename T >
  class ptrVectorHandling : public boost::ptr_vector< T > {
    public:
      ptrVectorHandling();      
      ptrVectorHandling(int const & size);
  };

  //----------------------------------------------------------------------------
  // cpp
  //----------------------------------------------------------------------------  
  template < typename T >
  ptrVectorHandling< T >::ptrVectorHandling(
  
  ) : boost::ptr_vector< T >() {
    
  }  
  
  template < typename T >
  ptrVectorHandling< T >::ptrVectorHandling(
    int const & size
  ) : boost::ptr_vector< T >(size) {
    
  }  
}

#endif	/* PTRVECTORHANDLING_H */

