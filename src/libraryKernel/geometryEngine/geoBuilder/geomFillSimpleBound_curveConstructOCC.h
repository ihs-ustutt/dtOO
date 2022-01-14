#ifndef geomFillSimpleBound_curveConstructOCC_H
#define	geomFillSimpleBound_curveConstructOCC_H

#include <dtOOTypeDef.h>

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  class dtSurface;
  class dtCurve2d;
  
  class geomFillSimpleBound_curveConstructOCC {
  public:
    dt__classOnlyName(geomFillSimpleBound_curveConstructOCC);
    geomFillSimpleBound_curveConstructOCC( 
      dtCurve const * const dtC
    );
    virtual ~geomFillSimpleBound_curveConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* geomFillSimpleBound_curveConstructOCC_H */

