#ifndef DTOCCBSPLINECURVE_H
#define	DTOCCBSPLINECURVE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve2d.h"

class Geom2d_BSplineCurve;

namespace dtOO {
  class dtOCCCurve2dBase; 
  
  class dtOCCBSplineCurve2d : public dtOCCCurve2d {
  public:
    dt__CLASSSTD(dtOCCBSplineCurve2d, dtCurve2d);
    dtOCCBSplineCurve2d();
    dtOCCBSplineCurve2d( dtOCCCurve2dBase const & orig);
    virtual ~dtOCCBSplineCurve2d();
    virtual dtCurve2d * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint2 controlPoint( int const nPoint ) const;
//    virtual void setControlPoint( int const nPoint, dtPoint2 const point );    
  private:
    Geom2d_BSplineCurve const * _ptr;
  };
}
#endif	/* DTOCCBSPLINECURVE_H */

