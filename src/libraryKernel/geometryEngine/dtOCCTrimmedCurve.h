#ifndef DTOCCTRIMMEDCURVE_H
#define	DTOCCTRIMMEDCURVE_H

#include <dtOOTypeDef.h>

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
    dtOCCTrimmedCurve(dtOCCCurveBase const & orig);
    virtual ~dtOCCTrimmedCurve();
    virtual dtCurve * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( int const nPoint ) const;
    virtual void setControlPoint( int const nPoint, dtPoint3 const point );  
  private:
    Geom_TrimmedCurve const * _ptr;
  };
}

#endif	/* DTOCCTRIMMEDCURVE_H */

