#ifndef DTOCCBEZIERCURVE_H
#define	DTOCCBEZIERCURVE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_BezierCurve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCBezierCurve : public dtOCCCurve {
  public:
    dt__CLASSSTD(dtOCCBezierCurve, dtCurve);
    dtOCCBezierCurve();
    dtOCCBezierCurve( dtOCCCurveBase const & orig);
    virtual ~dtOCCBezierCurve();
    virtual dtCurve * clone( void ) const;
    virtual int getOrder( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 getControlPoint3d( int const nPoint ) const;
    virtual void setControlPoint3d( int const nPoint, dtPoint3 const point );    
  private:
    Geom_BezierCurve const * _ptr;
  };
}
#endif	/* DTOCCBEZIERCURVE_H */

