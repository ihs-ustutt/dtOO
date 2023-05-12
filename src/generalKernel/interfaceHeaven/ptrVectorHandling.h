#ifndef PTRVECTORHANDLING_H
#define	PTRVECTORHANDLING_H

#include <dtOOTypeDef.h>

#include <boost/ptr_container/ptr_vector.hpp>

namespace dtOO {
  template < typename T >
  using ptrVectorHandling = boost::ptr_vector< T >;
}

#endif	/* PTRVECTORHANDLING_H */

