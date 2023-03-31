#ifndef lVHOstateHandler_H
#define	lVHOstateHandler_H

#include <mainConceptFwd.h>
#include <logMe/dtMacros.h>
#include "lVHOInterface.h"
#include "lVHOSubject.h"

namespace dtOO {
  class labelHandling;

  class lVHOstateHandler : public lVHOInterface {
    public:
      dt__classOnlyName(lVHOstateHandler);
      lVHOstateHandler( void );
      lVHOstateHandler( jsonPrimitive const & config, lVHOSubject * subj);
      virtual ~lVHOstateHandler();
      std::string commonState( void ); 
      std::string writeState( std::string label = "");
    protected:
      virtual lVHOSubject & subject( void );
      virtual lVHOSubject const & subject( void ) const;
    private:
      static lVHOSubject * _subj;
  };
}

#endif /* lVHOstateHandler_H */
