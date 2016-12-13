#ifndef DTOCCTRIMMEDCURVE_H
#define	DTOCCTRIMMEDCURVE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_TrimmedCurve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCTrimmedCurve : public dtOCCCurve {
  public:
    dt__class(dtOCCTrimmedCurve, dtCurve);    
    dtOCCTrimmedCurve();
    dtOCCTrimmedCurve(dtOCCCurveBase const & orig, float const u0, float const u1);
    virtual ~dtOCCTrimmedCurve();
    virtual dtCurve * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( int const nPoint ) const;
    virtual void setControlPoint( int const nPoint, dtPoint3 const point );  
  private:
    Geom_TrimmedCurve const * _ptr;
    float _u0;
    float _u1;
  };
}

#endif	/* DTOCCTRIMMEDCURVE_H */

