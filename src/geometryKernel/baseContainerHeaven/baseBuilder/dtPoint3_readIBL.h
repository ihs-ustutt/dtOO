#ifndef dtPoint3_readIBL_H
#define	dtPoint3_readIBL_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtPoint3_readIBL {
  public:
    dt__classOnlyName(dtPoint3_readIBL);
    dtPoint3_readIBL(std::string const & filename);
    virtual ~dtPoint3_readIBL();
    std::vector< dtPoint3 > result(void) const;
  private:
    std::vector< dtPoint3 > _pp;
  };
}
#endif	/* dtPoint3_readIBL_H */

