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
    dt__CLASSSTD(dtOCCBSplineCurve, dtCurve);
    dtOCCBSplineCurve();
    dtOCCBSplineCurve( dtOCCCurveBase const & orig);
    virtual ~dtOCCBSplineCurve();
    virtual dtCurve * clone( void ) const;
    virtual int getOrder( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 getControlPoint3d( int const nPoint ) const;
    virtual void setControlPoint3d( int const nPoint, dtPoint3 const point );    
  private:
    Geom_BSplineCurve const * _ptr;
  };
}
#endif	/* DTOCCBSPLINECURVE_H */

