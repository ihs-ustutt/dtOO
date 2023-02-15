#ifndef DTOCCLINE_H
#define	DTOCCLINE_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_Line;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCLine : public dtOCCCurve {
  public:
    dt__class(dtOCCLine, dtCurve);
    dtOCCLine();
    dtOCCLine( dtOCCCurveBase const & orig);
    virtual ~dtOCCLine();
    virtual dtOCCLine * clone( void ) const;
    virtual dtInt order( void ) const;
    virtual dtInt nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( dtInt const nPoint ) const;
    virtual void setControlPoint( dtInt const nPoint, dtPoint3 const point );    
  private:
    Geom_Line const * _ptr;
  };
}
#endif	/* DTOCCLINE_H */

