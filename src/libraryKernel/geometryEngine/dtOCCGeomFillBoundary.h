#ifndef dtOCCGeomFillBoundary_H
#define	dtOCCGeomFillBoundary_H

#include <dtOOTypeDef.h>

#include <interfaceHeaven/ptrHandling.h>
#include "dtOCCCurve.h"

class GeomFill_Boundary;

namespace dtOO {
  class dtOCCCurveBase;
  class dtOCCGeomFillBoundaryBase;
  
  class dtOCCGeomFillBoundary : public dtOCCCurve {
    public:
      dt__class(dtOCCGeomFillBoundary, dtCurve);
      dtOCCGeomFillBoundary();
      dtOCCGeomFillBoundary( 
        dtOCCCurveBase const & cBase, dtOCCGeomFillBoundaryBase const & bBase
      );
      virtual ~dtOCCGeomFillBoundary();
      virtual dtOCCGeomFillBoundary * clone( void ) const = 0;
      virtual dtReal minPara( dtInt const & dir ) const = 0;
      virtual dtReal maxPara( dtInt const & dir ) const = 0;    
      virtual dtInt order( void ) const;
      virtual bool closed( void ) const;
      virtual dtPoint3 point( dtReal const uu ) const = 0;
      virtual dtVector3 firstDer( dtReal const uu) const;    
      virtual dtInt nControlPoints( void ) const;
      virtual dtPoint3 controlPoint( dtInt const nPoint ) const;
      virtual void setControlPoint( dtInt const nPoint, dtPoint3 const point ); 
      virtual dtReal l_u( dtReal const uu ) const;
      virtual dtReal u_l( dtReal const length ) const;
      virtual dtReal reparam(dtPoint3 const point) const;
      virtual void revert( void );
      virtual void translate( dtVector3 const & tt );
      dtOCCGeomFillBoundaryBase const & OCCBoundaryRef( void ) const;
//      dtOCCGeomFillBoundaryBase & OCCBoundaryRef( void );      
    private:
      ptrHandling< dtOCCGeomFillBoundaryBase > _boundary;
      GeomFill_Boundary const * _ptr;
  };
  dt__H_addCloneForpVH(dtOCCGeomFillBoundary);
}
#endif	/* dtOCCGeomFillBoundary_H */

