#ifndef dtOCCGeomFillSimpleBound_H
#define	dtOCCGeomFillSimpleBound_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCGeomFillBoundary.h"

class GeomFill_SimpleBound;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCGeomFillSimpleBound : public dtOCCGeomFillBoundary {
    public:
      dt__class(dtOCCGeomFillSimpleBound, dtCurve);
      dtOCCGeomFillSimpleBound();
      dtOCCGeomFillSimpleBound( 
        dtOCCCurveBase const & cBase, dtOCCGeomFillBoundaryBase const & bBase
      );
      virtual ~dtOCCGeomFillSimpleBound();
      virtual dtOCCGeomFillSimpleBound * clone( void ) const;
      virtual dtReal minPara( dtInt const & dir ) const;
      virtual dtReal maxPara( dtInt const & dir ) const;    
      virtual dtPoint3 point( dtReal const uu ) const;
    private:
      GeomFill_SimpleBound const * _ptr;
  };
}
#endif	/* dtOCCGeomFillSimpleBound_H */

