#ifndef bSplineCurve_poleWeightKnotMultOrderConstructOCC_H
#define	bSplineCurve_poleWeightKnotMultOrderConstructOCC_H

#include <dtOOTypeDef.h>

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
      std::vector< dtReal > const & weight, 
      std::vector< dtReal > const & knot, 
      std::vector< dtInt > const & mult, 
      dtInt const & order,
      bool const & periodic
    );
    virtual ~bSplineCurve_poleWeightKnotMultOrderConstructOCC();
    dtCurve * result( void );
  private:
    dt__pH(dtCurve) _dtC;
  };
}
#endif	/* bSplineCurve_poleWeightKnotMultOrderConstructOCC_H */

