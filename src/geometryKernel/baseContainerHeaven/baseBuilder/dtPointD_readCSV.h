#ifndef dtPointD_readCSV_H
#define	dtPointD_readCSV_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtPointD_readCSV {
  public:
    dt__classOnlyName(dtPointD_readCSV);
    dtPointD_readCSV(std::string const & filename);
    virtual ~dtPointD_readCSV();
    std::vector< dtPointD > result(void) const;
  private:
    std::vector< dtPointD > _pp;
  };
}
#endif	/* dtPointD_readCSV_H */

