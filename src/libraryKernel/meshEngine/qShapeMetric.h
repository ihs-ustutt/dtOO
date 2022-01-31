#ifndef QSHAPEMETRIC_H
#define	QSHAPEMETRIC_H

#include <dtOOTypeDef.h>

#include <logMe/dtMacros.h>

class MElement;
class MTetrahedron;
class MPyramid;

namespace dtOO {
  class qShapeMetric {
  public:
    dt__classOnlyName(qShapeMetric);    
    qShapeMetric();
    virtual ~qShapeMetric();
    dtReal operator()( ::MElement const * const me );
  private:
    static dtReal calculatePyramid( ::MPyramid * pyr );
    static dtReal calculateTetrahedron( ::MTetrahedron * tet );
  };
}
#endif	/* QSHAPEMETRIC_H */

