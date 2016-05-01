#ifndef QSHAPEMETRIC_H
#define	QSHAPEMETRIC_H

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
    float operator()( ::MElement const * const me );
  private:
    static float calculatePyramid( ::MPyramid * pyr );
    static float calculateTetrahedron( ::MTetrahedron * tet );
  };
}
#endif	/* QSHAPEMETRIC_H */

