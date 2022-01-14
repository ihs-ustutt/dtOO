#ifndef BVOSUBJECT_H
#define	BVOSUBJECT_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class bVOInterface;
  class boundedVolume;
  
  class bVOSubject {
    public:
      dt__classOnlyName(bVOSubject);
      bVOSubject();
      virtual ~bVOSubject();
      void attachBVObserver( bVOInterface* observer );
  //    void detachBVObserver( bVOInterface* observer );
      void preNotify( void );
      void postNotify( void );
    private:
      vectorHandling< bVOInterface * > _observers;
    private:

  };
}
#endif	/* BVOSUBJECT_H */

