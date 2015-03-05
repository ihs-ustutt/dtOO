#ifndef bSplineCurve2d_normalOffsetGeomCurve2dOCC_H
#define	bSplineCurve2d_normalOffsetGeomCurve2dOCC_H

#include <dtLinearAlgebra.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>

namespace dtOO {
  class dtCurve2d;
  
  class bSplineCurve2d_normalOffsetGeomCurve2dOCC {
  public:
    dt__CLASSNAME(bSplineCurve2d_normalOffsetGeomCurve2dOCC);    
    bSplineCurve2d_normalOffsetGeomCurve2dOCC(
      dtCurve2d const * const dtC, std::vector< float > const & offset
    );
    bSplineCurve2d_normalOffsetGeomCurve2dOCC(
      dtCurve2d const * const dtC, float const & offset, int const & nPoints
    );    
    virtual ~bSplineCurve2d_normalOffsetGeomCurve2dOCC();
    dtCurve2d * result( void );
  private:
    ptrHandling<dtCurve2d> _dtC2d;    
  };
}

#endif	/* bSplineCurve2d_normalOffsetGeomCurve2dOCC_H */

