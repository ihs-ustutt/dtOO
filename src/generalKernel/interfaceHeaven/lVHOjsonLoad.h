#ifndef lVHOjsonLoad_H
#define	lVHOjsonLoad_H

#include <mainConceptFwd.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include "lVHOInterface.h"
#include "lVHOSubject.h"

namespace dtOO {
  class labelHandling;

  class lVHOjsonLoad : public lVHOInterface {
    public:
      dt__class(lVHOjsonLoad, lVHOInterface);
      lVHOjsonLoad( void );
      lVHOjsonLoad( jsonPrimitive const & config, lVHOSubject * subj);
      virtual ~lVHOjsonLoad();
      virtual void observeSet( labelHandling * lH );
    private:
      dt__pH(lvH_constValue) _cV;
  };
}

#endif /* lVHOjsonLoad_H */
