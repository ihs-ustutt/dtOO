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
