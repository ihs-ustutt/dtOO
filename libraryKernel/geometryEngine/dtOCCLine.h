#ifndef DTOCCLINE_H
#define	DTOCCLINE_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_Line;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCLine : public dtOCCCurve {
  public:
    dt__CLASSSTD(dtOCCLine, dtCurve);
    dtOCCLine();
    dtOCCLine( dtOCCCurveBase const & orig);
    virtual ~dtOCCLine();
    virtual dtCurve * clone( void ) const;
    virtual int getOrder( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 getControlPoint3d( int const nPoint ) const;
    virtual void setControlPoint3d( int const nPoint, dtPoint3 const point );    
  private:
    Geom_Line const * _ptr;
  };
}
#endif	/* DTOCCLINE_H */

