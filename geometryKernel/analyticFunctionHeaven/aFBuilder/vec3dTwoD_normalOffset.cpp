#include "vec3dTwoD_normalOffset.h"

#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include "dtPoint3_vec3dTwoD.h"
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>

namespace dtOO {
  vec3dTwoD_normalOffset::vec3dTwoD_normalOffset(
    vec3dTwoD const * const v3d, vec3dTwoD const * const thick,
    dtVector3 const & nf, int const & nU, int const & nV,
    int const & order
  ) {
    twoDArrayHandling< dtPoint2 > uGrid = dtLinearAlgebra::unitGrid(nU, nV);
    twoDArrayHandling< dtPoint3 > grid(nU, nV);
    
    vectorHandling< dtCurve const * > cL(nV);
		dt__forAllIndex(uGrid, ii) {
      dt__forAllIndex(uGrid[ii], jj) {
        dtPoint3 ttUV 
        = 
        thick->YdtPoint3Percent( analyticFunction::aFXTwoD( uGrid[ii][jj] ) );
        float tt = ttUV.x();
        aFX uvT = analyticFunction::aFXTwoD(ttUV.y(), ttUV.z());

        dtPoint3 yy = v3d->YdtPoint3Percent(uvT);
        dtVector3 nn = v3d->unitNdtVector3Percent( uvT );		
        nn = nn - dtLinearAlgebra::dotProduct(nn, nf) * nf;
        dtLinearAlgebra::normalize(nn);		

        grid[ii][jj] = yy + tt * nn;
      }
    }
    dt__forFromToIndex(0, grid.size(1), jj) {
      cL[jj] = bSplineCurve_pointConstructOCC(grid.fixJ(jj), order).result();
    }
    
    _v3d.reset(
      new vec3dSurfaceTwoD(
        dt__tmpPtr(dtSurface, bSplineSurface_skinConstructOCC(cL).result())
      )
    );
    cL.destroy();
  }

  vec3dTwoD_normalOffset::~vec3dTwoD_normalOffset() {
  }

  vec3dTwoD * vec3dTwoD_normalOffset::result(void) {
    return _v3d->clone();
  }
}
