#ifndef ONEDIMTRIMMABLE_H
#define	ONEDIMTRIMMABLE_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class oneDimTrimmable {
    dt__CLASSNAME(oneDimTrimmable);
  public:  
    oneDimTrimmable();
    oneDimTrimmable(const oneDimTrimmable& orig);
    virtual ~oneDimTrimmable();
    virtual void trim( float const uuStart, float const uuEnd ) = 0;
  private:

  };
}
#endif	/* ONEDIMTRIMMABLE_H */

