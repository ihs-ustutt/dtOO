#ifndef DTOCCLINE2D_H
#define	DTOCCLINE2D_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve2d.h"

class Geom2d_Line;

namespace dtOO {
  class dtOCCCurve2dBase; 
  
  class dtOCCLine2d : public dtOCCCurve2d {
  public:
    dt__CLASSSTD(dtOCCLine2d, dtCurve2d);    
    dtOCCLine2d();
    dtOCCLine2d(dtOCCCurve2dBase const & orig);
    virtual ~dtOCCLine2d();
    virtual dtCurve2d * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint2 controlPoint( int const nPoint ) const;
  private:
    Geom2d_Line const * _ptr;
  };
}

#endif	/* DTOCCLINE2D_H */

