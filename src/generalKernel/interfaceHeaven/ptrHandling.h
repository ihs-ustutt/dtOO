#ifndef PTRHANDLING_H
#define	PTRHANDLING_H

#include <dtOOTypeDef.h>

#include <boost/shared_ptr.hpp>

namespace dtOO {
  template < typename T >
  using ptrHandling = boost::shared_ptr< T >;
}

#endif	/* PTRHANDLING_H */

