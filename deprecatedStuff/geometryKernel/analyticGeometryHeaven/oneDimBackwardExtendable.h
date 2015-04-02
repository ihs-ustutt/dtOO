#ifndef ONEDIMBACKWARDEXTENDABLE_H
#define	ONEDIMBACKWARDEXTENDABLE_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class oneDimBackwardExtendable {
    public:    
      dt__classOnlyName(oneDimBackwardExtendable);
      oneDimBackwardExtendable();
      oneDimBackwardExtendable(const oneDimBackwardExtendable& orig);
      virtual ~oneDimBackwardExtendable();
      virtual void backwardExtend( void ) = 0;
    private:

  };
}
#endif	/* ONEDIMBACKWARDEXTENDABLE_H */