#ifndef DTOCCBSPLINECURVE_H
#define	DTOCCBSPLINECURVE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_BSplineCurve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCBSplineCurve : public dtOCCCurve {
  public:
    dt__class(dtOCCBSplineCurve, dtCurve);
    dtOCCBSplineCurve();
    dtOCCBSplineCurve( dtOCCCurveBase const & orig);
    virtual ~dtOCCBSplineCurve();
    virtual dtCurve * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( int const nPoint ) const;
    virtual void setControlPoint( int const nPoint, dtPoint3 const point );
    virtual std::string dumpToString(void) const;
  private:
    Geom_BSplineCurve const * _ptr;
  };
}
#endif	/* DTOCCBSPLINECURVE_H */
