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

#include "lVHOSubject.h"

#include <logMe/logMe.h>

namespace dtOO {  
  lVHOSubject::lVHOSubject() {
  }
  
  lVHOSubject::~lVHOSubject() {
  }

  void lVHOSubject::attach(lVHOInterface * observer) {
    dt__debug(attach(), << "Call");
  }

  void lVHOSubject::letObserve( lVHOInterface * callerToObserve ) {
    dt__debug(letObserve(), << "Call");
  }

  std::vector< labelHandling * > lVHOSubject::internalStdVector( void ) {
    dt__throw(internalStdVector(), << "Call");
  }

  dt__pVH(lVHOInterface) const & lVHOSubject::refObserver( void ) const {
    dt__throw(refObserver(), << "Call");
  }
}
