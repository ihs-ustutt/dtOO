#ifndef bSplineCurve_poleWeightKnotMultOrderConstructOCC_H
#define	bSplineCurve_poleWeightKnotMultOrderConstructOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve;
  
  class bSplineCurve_poleWeightKnotMultOrderConstructOCC {
  public:
    dt__classOnlyName(bSplineCurve_poleWeightKnotMultOrderConstructOCC);
    bSplineCurve_poleWeightKnotMultOrderConstructOCC( 
      std::vector< dtPoint3 > const & pole, 
      std::vector< float > const & weight, 
      std::vector< float > const & knot, 
      std::vector< int > const & mult, 
      int const & order,
      bool const & periodic
    );
    virtual ~bSplineCurve_poleWeightKnotMultOrderConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_poleWeightKnotMultOrderConstructOCC_H */

