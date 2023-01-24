#ifndef lVHOjsonLoad_H
#define	lVHOjsonLoad_H

#include <mainConceptFwd.h>
#include <logMe/dtMacros.h>
#include "lVHOInterface.h"
#include "lVHOSubject.h"

namespace dtOO {
  class labelHandling;

  class lVHOjsonLoad : public lVHOInterface {
    public:
      dt__classOnlyName(lVHOjsonLoad);
      lVHOjsonLoad( lVHOSubject * subj, jsonPrimitive const & config );
      virtual ~lVHOjsonLoad();
      virtual void observeSet( labelHandling * lH );
    private:
      dt__pH(lvH_constValue) _cV;
  };
}

#endif /* lVHOjsonLoad_H */
