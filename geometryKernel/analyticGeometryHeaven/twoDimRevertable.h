#ifndef TWODIMREVERTABLE_H
#define	TWODIMREVERTABLE_H

#include <logMe/dtMacros.h>

namespace dtOO {
  class twoDimRevertable {
    public:    
      dt__CLASSNAME(twoDimRevertable);
      twoDimRevertable();
      twoDimRevertable(const twoDimRevertable& orig);
      virtual ~twoDimRevertable();
      virtual void revert( void ) = 0;  
    private:

  };
}
#endif	/* TWODIMREVERTABLE_H */

