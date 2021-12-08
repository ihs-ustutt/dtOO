#ifndef dtOCCGeomFillBoundary_H
#define	dtOCCGeomFillBoundary_H

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
      virtual dtCurve * clone( void ) const = 0;
      virtual float minPara( int const & dir ) const = 0;
      virtual float maxPara( int const & dir ) const = 0;    
      virtual int order( void ) const;
      virtual bool closed( void ) const;
      virtual dtPoint3 point( float const uu ) const = 0;
      virtual dtVector3 firstDer( float const uu) const;    
      virtual int nControlPoints( void ) const;
      virtual dtPoint3 controlPoint( int const nPoint ) const;
      virtual void setControlPoint( int const nPoint, dtPoint3 const point ); 
      virtual float l_u( float const uu ) const;
      virtual float u_l( float const length ) const;
      virtual float reparam(dtPoint3 const point) const;
      virtual void revert( void );
      virtual void translate( dtVector3 const & tt );
      dtOCCGeomFillBoundaryBase const & OCCBoundaryRef( void ) const;
//      dtOCCGeomFillBoundaryBase & OCCBoundaryRef( void );      
    private:
      ptrHandling< dtOCCGeomFillBoundaryBase > _boundary;
      GeomFill_Boundary const * _ptr;
  };
}
#endif	/* dtOCCGeomFillBoundary_H */

