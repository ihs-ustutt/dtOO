#ifndef dtPoint3_readCSV_H
#define	dtPoint3_readCSV_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>

namespace dtOO {
  class dtPoint3_readCSV {
  public:
    dt__classOnlyName(dtPoint3_readCSV);
    dtPoint3_readCSV(std::string const & filename);
    virtual ~dtPoint3_readCSV();
    std::vector< dtPoint3 > result(void) const;
  private:
    std::vector< dtPoint3 > _pp;
  };
}
#endif	/* dtPoint3_readCSV_H */

