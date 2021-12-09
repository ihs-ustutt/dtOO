#ifndef dtOCCGeomFillSimpleBound_H
#define	dtOCCGeomFillSimpleBound_H

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
      virtual dtCurve * clone( void ) const;
      virtual float minPara( int const & dir ) const;
      virtual float maxPara( int const & dir ) const;    
      virtual dtPoint3 point( float const uu ) const;
    private:
      GeomFill_SimpleBound const * _ptr;
  };
}
#endif	/* dtOCCGeomFillSimpleBound_H */

