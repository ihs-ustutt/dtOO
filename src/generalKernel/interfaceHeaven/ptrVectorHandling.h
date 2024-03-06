/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#ifndef PTRVECTORHANDLING_H
#define	PTRVECTORHANDLING_H

#include <dtOOTypeDef.h>

#include <boost/ptr_container/ptr_vector.hpp>

namespace dtOO {
  template < typename T >
  using ptrVectorHandling = boost::ptr_vector< T >;
}

#endif	/* PTRVECTORHANDLING_H */
