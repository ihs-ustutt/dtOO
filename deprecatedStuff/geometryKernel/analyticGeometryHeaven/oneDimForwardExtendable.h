#ifndef ONEDIMFORWARDEXTENDABLE_H
#define	ONEDIMFORWARDEXTENDABLE_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class oneDimForwardExtendable {
    public:    
      dt__classOnlyName(oneDimForwardExtendable);
      oneDimForwardExtendable();
      oneDimForwardExtendable(const oneDimForwardExtendable& orig);
      virtual ~oneDimForwardExtendable();
      virtual void forwardExtend( void ) = 0;
    private:

  };
}
#endif	/* ONEDIMFORWARDEXTENDABLE_H */