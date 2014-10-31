#ifndef SURFACEBLENDCONSTRUCT_H
#define	SURFACEBLENDCONSTRUCT_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class surfaceBlendConstruct {
  public:
    dt__CLASSNAME(surfaceBlendConstruct);     
    surfaceBlendConstruct( vectorHandling< dtCurve const * > const &  curveList );
    virtual ~surfaceBlendConstruct();
    dtSurface * result( void );
  private:
    vectorHandling< dtCurve const * > reorderForBlending( 
        vectorHandling< dtCurve const * > const * const curveV 
    ) const;
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* SURFACEBLENDCONSTRUCT_H */

