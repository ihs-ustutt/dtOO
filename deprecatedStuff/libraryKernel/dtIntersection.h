#ifndef DTINTERSECTION_H
#define	DTINTERSECTION_H

#include "dtSislBase.h"
#include "dtSislSurf.h"
#include "dtCurve.h"

#include <logMe/dtMacros.h>

class SISLIntcurve;

namespace dtOO {
  class dtIntersection : public dtSislBase {
  public:
    DTCLASSNAMEMETHOD(dtIntersection);
    DTCLASSLOGMETHOD(dtIntersection);    
    dtIntersection();
//    dtIntersection(const dtIntersection& orig);
    dtIntersection( dtSislSurf const & surf1, dtSislSurf const & surf2 );
    void dump( void ) const;
    int getNCurves( void ) const;
    dtCurve getCurve( int const num ) const;
    dtCurve getEquiResplinedCurve( int const cId, int const sId, int const nPoints, int const order ) const;
    virtual ~dtIntersection();
  private:
    int _nIntPt;
    double * _uPt;
    double * _vPt;
    int _nIntCr;
    SISLIntcurve ** _intCurves;
    dtSislSurf _surf[2];
  };
}
#endif	/* DTINTERSECTION_H */

