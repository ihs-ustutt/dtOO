#ifndef IGESWRITABLE_H
#define	IGESWRITABLE_H

#include <vector>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtSislBase;
  
  class igesWritable {
  public:
    DTCLASSNAMEMETHOD(igesWritable);
    DTCLASSLOGMETHOD(igesWritable);
    igesWritable();
    igesWritable(const igesWritable& orig);
    virtual ~igesWritable();
    virtual std::vector< dtSislBase * > getDtSislObjects( void ) = 0;
  private:

  };
}
#endif	/* IGESWRITABLE_H */

