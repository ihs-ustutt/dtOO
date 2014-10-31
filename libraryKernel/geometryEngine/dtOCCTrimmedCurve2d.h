#ifndef DTOCCTRIMMEDCURVE2D_H
#define	DTOCCTRIMMEDCURVE2D_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve2d.h"

class Geom2d_TrimmedCurve;

namespace dtOO {
  class dtOCCCurve2dBase; 
  
  class dtOCCTrimmedCurve2d : public dtOCCCurve2d {
  public:
    dt__CLASSSTD(dtOCCTrimmedCurve2d, dtCurve2d);    
    dtOCCTrimmedCurve2d();
    dtOCCTrimmedCurve2d(dtOCCCurve2dBase const & orig);
    virtual ~dtOCCTrimmedCurve2d();
    virtual dtCurve2d * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint2 controlPoint( int const nPoint ) const;
  private:
    Geom2d_TrimmedCurve const * _ptr;
  };
}

#endif	/* DTOCCTRIMMEDCURVE2D_H */

