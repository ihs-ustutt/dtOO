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

#ifndef lVHOSubject_H
#define	lVHOSubject_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class lVHOInterface;
  class labelHandling;

  class lVHOSubject {
    public:
      dt__classOnlyName(lVHOSubject);
      lVHOSubject();
      virtual ~lVHOSubject();
      virtual void attach(lVHOInterface * observer);
      virtual void letObserve( lVHOInterface * callerToObserve = NULL );
      virtual std::vector< labelHandling * > internalStdVector( void );
      virtual dt__pVH(lVHOInterface) const & refObserver( void ) const;
  };
}

#endif /* lVHOSubject_H */
