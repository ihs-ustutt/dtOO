#ifndef ONEDIMREVERTABLE_H
#define	ONEDIMREVERTABLE_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class oneDimRevertable {
  public:
    dt__CLASSNAME(oneDimRevertable);    
    oneDimRevertable();
    oneDimRevertable(const oneDimRevertable& orig);
    virtual ~oneDimRevertable();
    virtual void revert( void ) = 0;
  private:

  };
}
#endif	/* ONEDIMREVERTABLE_H */

