#ifndef DTOCCTRIMMEDCURVE2D_H
#define	DTOCCTRIMMEDCURVE2D_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve2d.h"

class Geom2d_TrimmedCurve;

namespace dtOO {
  class dtOCCCurve2dBase; 
  
  class dtOCCTrimmedCurve2d : public dtOCCCurve2d {
  public:
    dt__class(dtOCCTrimmedCurve2d, dtCurve2d);    
    dtOCCTrimmedCurve2d();
    dtOCCTrimmedCurve2d(dtOCCCurve2dBase const & orig, dtReal const u0, dtReal const u1);
    virtual ~dtOCCTrimmedCurve2d();
    virtual dtOCCTrimmedCurve2d * clone( void ) const;
    virtual dtInt order( void ) const;
    virtual dtInt nControlPoints( void ) const;
    virtual dtPoint2 controlPoint( dtInt const nPoint ) const;
    dtReal getU0( void ) const;
    dtReal getU1( void ) const;
  private:
    Geom2d_TrimmedCurve const * _ptr;
    dtReal _u0;
    dtReal _u1;    
  };
}

#endif	/* DTOCCTRIMMEDCURVE2D_H */

