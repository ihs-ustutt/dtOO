#ifndef ONEDIMCLOSEABLE_H
#define	ONEDIMCLOSEABLE_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class oneDimCloseable {    
  public:
    dt__CLASSNAME(oneDimCloseable);
    oneDimCloseable();
    oneDimCloseable(const oneDimCloseable& orig);
    virtual ~oneDimCloseable();
    virtual void closeArithmetic( void ) = 0;
    virtual void closeStraight( void ) = 0;
  private:

  };
}
#endif	/* ONEDIMCLOSEABLE_H */

