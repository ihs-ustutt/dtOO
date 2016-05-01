#include "vec3dTwoD_normalOffset.h"

#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
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
    
//    dt__quickdebug(
//      << logMe::dtFormat("%4s %4s %12s %12s %12s %12s") 
//        % "ii" % "jj"
//        % "tt"
//        % "cNf"
//        % "|nn-nnOld|"
//        % "nn.nnOld"
//    );    
		dt__forAllIndex(uGrid[0], jj) {
//      dtVector3 nnOld(0,0,0);
      dt__forAllIndex(uGrid, ii) {
        dtPoint3 ttUV 
        = 
        thick->YdtPoint3Percent( analyticFunction::aFXTwoD( uGrid[ii][jj] ) );
        float tt = ttUV.x();
        aFX uvT = analyticFunction::aFXTwoD(ttUV.y(), ttUV.z());

        dtPoint3 yy = v3d->YdtPoint3Percent(uvT);
        dtVector3 nn = v3d->unitNdtVector3Percent( uvT );		
        float cNf = dtLinearAlgebra::dotProduct(nn, nf);
        nn = nn - cNf * nf;
        dtLinearAlgebra::normalize(nn);
        
        //
        // change of nn
        //
//        dt__quickdebug(
//          << logMe::dtFormat("%4i %4i %12.4f %12.4f %12.4f %12.4f") 
//            % ii % jj
//            % tt
//            % cNf
//            % dtLinearAlgebra::length(nn-nnOld)
//            % dtLinearAlgebra::dotProduct(nn,nnOld)
//        );        
//        nnOld = nn;
        
        grid[ii][jj] = yy + tt * nn;
      }
    }
    dt__forFromToIndex(0, grid.size(1), jj) {
      cL[jj] = bSplineCurve_pointConstructOCC(grid.fixJ(jj), order).result();
      _vC.push_back( 
        new vec3dCurveOneD( 
          dt__tmpPtr(
            dtCurve, bSplineCurve_pointConstructOCC(
              grid.fixJ(jj), 1
            ).result()
          )
        ) 
      );
    }
    
    _vS.reset(
      new vec3dSurfaceTwoD(
        dt__tmpPtr(dtSurface, bSplineSurface_skinConstructOCC(cL).result())
      )
    );
    cL.destroy();
  }

  vec3dTwoD_normalOffset::~vec3dTwoD_normalOffset() {
  }

  vec3dTwoD * vec3dTwoD_normalOffset::result(void) const {
    return _vS->clone();
  }

  dt__pVH(vec3dOneD) vec3dTwoD_normalOffset::resultWire(void) const {
    return _vC;
  }  
}
