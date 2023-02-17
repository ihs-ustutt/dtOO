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
      dt__class(bVOSubject, bVOSubject);
      bVOSubject();
      virtual ~bVOSubject();
      void attachBVObserver( bVOInterface* observer );
      void preNotify( void );
      void postNotify( void );
      void dump( void ) const;
    private:
      vectorHandling< bVOInterface * > _observers;
  };
}
#endif	/* BVOSUBJECT_H */

