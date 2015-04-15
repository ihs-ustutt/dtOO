#ifndef SURFACEPOINTCONSTRUCT_H
#define	SURFACEPOINTCONSTRUCT_H

#include <dtLinearAlgebra.h>
#include <interfaceHeaven/ptrHandling.h>
#include <math.h>

namespace dtOO {
  class dtSurface;
  class dtCurve;
  
  class surfacePointConstruct {
  public:
    dt__classOnlyName(surfacePointConstruct);   
    surfacePointConstruct(
      std::vector< std::vector< dtPoint3 > > const & points, 
      int const & orderOne, 
      int const & orderTwo
    );
    virtual ~surfacePointConstruct();
    dtSurface * result( void );
  private:
    ptrHandling<dtSurface> _dtS;
  };
}
#endif	/* SURFACEPOINTCONSTRUCT_H */

