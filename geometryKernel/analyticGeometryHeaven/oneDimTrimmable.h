#ifndef ONEDIMTRIMMABLE_H
#define	ONEDIMTRIMMABLE_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class oneDimTrimmable {
  public:  
    dt__CLASSNAME(oneDimTrimmable);    
    oneDimTrimmable();
    oneDimTrimmable(const oneDimTrimmable& orig);
    virtual ~oneDimTrimmable();
    virtual void trim( float const uuStart, float const uuEnd ) = 0;
  private:

  };
}
#endif	/* ONEDIMTRIMMABLE_H */

