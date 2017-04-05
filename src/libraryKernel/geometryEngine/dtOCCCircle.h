#ifndef dtOCCCircle_H
#define	dtOCCCircle_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_Curve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCCircle : public dtOCCCurve {
  public:
    dt__class(dtOCCCircle, dtCurve);    
    dtOCCCircle();
    dtOCCCircle(dtOCCCurveBase const & orig);
    virtual ~dtOCCCircle();
    virtual dtCurve * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( int const nPoint ) const;
    virtual void setControlPoint( int const nPoint, dtPoint3 const point );  
//    dtCurve * bSplineCurve( void ) const;
  private:
    Geom_Curve const * _ptr;
  };
}

#endif	/* dtOCCCircle_H */

