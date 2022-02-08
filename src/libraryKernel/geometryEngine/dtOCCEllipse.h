#ifndef dtOCCEllipse_H
#define	dtOCCEllipse_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_Curve;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCEllipse : public dtOCCCurve {
  public:
    dt__class(dtOCCEllipse, dtCurve);    
    dtOCCEllipse();
    dtOCCEllipse(dtOCCCurveBase const & orig);
    virtual ~dtOCCEllipse();
    virtual dtCurve * clone( void ) const;
    virtual dtInt order( void ) const;
    virtual dtInt nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( dtInt const nPoint ) const;
    virtual void setControlPoint( dtInt const nPoint, dtPoint3 const point );  
  private:
    Geom_Curve const * _ptr;
  };
}

#endif	/* dtOCCEllipse_H */

