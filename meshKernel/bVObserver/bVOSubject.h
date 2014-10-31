#ifndef BVOSUBJECT_H
#define	BVOSUBJECT_H

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>

#include <list>

namespace dtOO {
  class bVOInterface;
  class boundedVolume;
  
  class bVOSubject {
  public:
    dt__CLASSNAME(bVOSubject);
    bVOSubject();
    virtual ~bVOSubject();
    void attachBVObserver( bVOInterface* observer );
//    void detachBVObserver( bVOInterface* observer );
    void notify( void );
private:
    vectorHandling< bVOInterface * > _observers;
  private:

  };
}
#endif	/* BVOSUBJECT_H */

