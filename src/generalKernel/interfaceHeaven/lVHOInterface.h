#ifndef lVHOInterface_H
#define	lVHOInterface_H

#include <mainConceptFwd.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class lVHOSubject;
  class labelHandling;
  class jsonPrimitive;

  class lVHOInterface {
    public:
      dt__classOnlyName(lVHOInterface);
      lVHOInterface( lVHOSubject * subj, jsonPrimitive const & config );
      lVHOInterface( jsonPrimitive const & config );
      virtual ~lVHOInterface();
      virtual void observeSet( labelHandling * lH );
      virtual void observe( labelHandling * lH );
    protected:
      jsonPrimitive & config( void );
      jsonPrimitive const & config( void ) const;
    private:
      dt__pH(jsonPrimitive) _config;
  };
}

#endif /* lVHOInterface_H */
