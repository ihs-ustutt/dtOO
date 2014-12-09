#ifndef DTOCCCONIC_H
#define	DTOCCCONIC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include "dtOCCCurve.h"

class Geom_Conic;

namespace dtOO {
  class dtOCCCurveBase; 
  
  class dtOCCConic : public dtOCCCurve {
  public:
    dt__CLASSSTD(dtOCCConic, dtCurve);    
    dtOCCConic();
    dtOCCConic(dtOCCCurveBase const & orig);
    virtual ~dtOCCConic();
    virtual dtCurve * clone( void ) const;
    virtual int order( void ) const;
    virtual int nControlPoints( void ) const;
    virtual dtPoint3 controlPoint( int const nPoint ) const;
    virtual void setControlPoint( int const nPoint, dtPoint3 const point );  
//    dtCurve * bSplineCurve( void ) const;
  private:
    Geom_Conic const * _ptr;
  };
}

#endif	/* DTOCCCONIC_H */

