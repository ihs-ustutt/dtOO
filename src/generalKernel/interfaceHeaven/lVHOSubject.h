#ifndef lVHOSubject_H
#define	lVHOSubject_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class lVHOInterface;

  class lVHOSubject {
    public:
      dt__classOnlyName(lVHOSubject);
      lVHOSubject();
      virtual ~lVHOSubject();
      virtual void attach(lVHOInterface * observer);
      virtual void letObserve( lVHOInterface * callerToObserve = NULL );
  };
}

#endif /* lVHOSubject_H */
