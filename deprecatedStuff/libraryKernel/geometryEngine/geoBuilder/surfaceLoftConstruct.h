#ifndef SURFACELOFTCONSTRUCT_H
#define	SURFACELOFTCONSTRUCT_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <interfaceHeaven/vectorHandling.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class surfaceLoftConstruct {
  public:
    dt__CLASSNAME(surfaceLoftConstruct);
    surfaceLoftConstruct( vectorHandling< dtCurve const * > const &  curveList, int const order );
    virtual ~surfaceLoftConstruct();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* SURFACELOFTCONSTRUCT_H */

