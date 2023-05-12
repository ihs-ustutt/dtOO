#ifndef dtOCCGeomFillBoundWithSurf_H
#define	dtOCCGeomFillBoundWithSurf_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCGeomFillBoundary.h"

class GeomFill_BoundWithSurf;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCGeomFillBoundWithSurf : public dtOCCGeomFillBoundary {
    public:
      dt__class(dtOCCGeomFillBoundWithSurf, dtCurve);
      dtOCCGeomFillBoundWithSurf();
      dtOCCGeomFillBoundWithSurf( 
        dtOCCCurveBase const & cBase, dtOCCGeomFillBoundaryBase const & bBase
      );
      virtual ~dtOCCGeomFillBoundWithSurf();
      virtual dtOCCGeomFillBoundWithSurf * clone( void ) const;
      virtual dtReal minPara( dtInt const & dir ) const;
      virtual dtReal maxPara( dtInt const & dir ) const;    
      virtual dtPoint3 point( dtReal const uu ) const;
    private:
      GeomFill_BoundWithSurf const * _ptr;
  };
}
#endif	/* dtOCCGeomFillBoundWithSurf_H */

