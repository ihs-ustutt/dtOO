#ifndef DTOCCBSPLINECURVE2D_H
#define	DTOCCBSPLINECURVE2D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve2d.h"

class Geom2d_BSplineCurve;

namespace dtOO {
  class dtOCCCurve2dBase; 
  
  class dtOCCBSplineCurve2d : public dtOCCCurve2d {
  public:
    dt__class(dtOCCBSplineCurve2d, dtCurve2d);
    dtOCCBSplineCurve2d();
    dtOCCBSplineCurve2d( dtOCCCurve2dBase const & orig);
    virtual ~dtOCCBSplineCurve2d();
    virtual dtCurve2d * clone( void ) const;
    virtual dtInt order( void ) const;
    virtual dtInt nControlPoints( void ) const;
    virtual dtPoint2 controlPoint( dtInt const nPoint ) const;
    virtual void setControlPoint( dtInt const nPoint, dtPoint2 const point );    
  private:
    Geom2d_BSplineCurve const * _ptr;
  };
}
#endif	/* DTOCCBSPLINECURVE2D_H */

