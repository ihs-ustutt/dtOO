#ifndef PTRHANDLING_H
#define	PTRHANDLING_H

#include <boost/scoped_ptr.hpp>

namespace dtOO {
  template < typename T >
  class ptrHandling : public boost::scoped_ptr< T > {
    public:
      ptrHandling();
      ptrHandling( T * ptr );
      bool isNull( void ) const;
  };

  //----------------------------------------------------------------------------
  // cpp
  //----------------------------------------------------------------------------
  template < typename T >
  ptrHandling< T >::ptrHandling() : boost::scoped_ptr< T >() {
  }

  template < typename T >
  ptrHandling< T >::ptrHandling( T * ptr ) : boost::scoped_ptr< T >(ptr) {
  }

  template < typename T >
  bool ptrHandling< T >::isNull( void ) const {
    if ( this->get() == NULL ) return true;
    return false;
  }  
}

#endif	/* PTRHANDLING_H */

