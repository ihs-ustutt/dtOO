#ifndef FLOAT_SCAONEDPOINT_H
#define	FLOAT_SCAONEDPOINT_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>
#include <dtLinearAlgebra.h>
#include <vector>

namespace dtOO {
  class scaOneD;
  
  class float_scaOneDPoint {
  public:
    float_scaOneDPoint(
      scaOneD const * const s1d, int const & nPoints
    );
    virtual ~float_scaOneDPoint();
    std::vector< dtReal > result( void );
  private:
    std::vector< dtReal > _ff;
  };
}
#endif	/* FLOAT_SCAONEDPOINT_H */

