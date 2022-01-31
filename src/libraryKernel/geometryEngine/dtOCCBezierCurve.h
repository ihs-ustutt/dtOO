#ifndef DTOCCBEZIERCURVE_H
#define	DTOCCBEZIERCURVE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_BezierCurve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCBezierCurve : public dtOCCCurve {
  public:
    dt__class(dtOCCBezierCurve, dtCurve);
    dtOCCBezierCurve();
    dtOCCBezierCurve( dtOCCCurveBase const & orig);
    virtual ~dtOCCBezierCurve();
    virtual dtCurve * clone( void ) const;
    virtual dtInt order( void ) const;
    virtual dtInt nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( dtInt const nPoint ) const;
    virtual void setControlPoint( dtInt const nPoint, dtPoint3 const point );    
  private:
    Geom_BezierCurve const * _ptr;
  };
}
#endif	/* DTOCCBEZIERCURVE_H */

