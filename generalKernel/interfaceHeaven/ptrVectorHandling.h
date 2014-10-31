#ifndef PTRVECTORHANDLING_H
#define	PTRVECTORHANDLING_H

#include <boost/ptr_container/ptr_vector.hpp>

namespace dtOO {
  template < typename T >
  class ptrVectorHandling : public boost::ptr_vector< T > {
  };
}

#endif	/* PTRVECTORHANDLING_H */

