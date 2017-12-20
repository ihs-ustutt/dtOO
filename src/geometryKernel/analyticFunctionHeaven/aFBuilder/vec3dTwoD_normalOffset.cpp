#include "vec3dTwoD_normalOffset.h"

#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>

#include <boost/assign.hpp>

namespace dtOO {
  vec3dTwoD_normalOffset::vec3dTwoD_normalOffset(
    vec3dTwoD const * const v3d, vec3dTwoD const * const thick,
    dtVector3 const & nf, int const & nU, int const & nV,
    int const & order,
    int const & skinOrderMin, int const & skinOrderMax,
    int const & skinNIterations          
  ) {
    
    dt__pH(vec3dTwoD) cV3d( v3d->clone() );
    
    twoDArrayHandling< dtPoint2 > uGrid = dtLinearAlgebra::unitGrid(nU, nV);
    twoDArrayHandling< dtPoint3 > grid(nU, nV);

    vectorHandling< dtCurve const * > cL(nV);

    dt__forAllIndex(uGrid[0], jj) {
      dt__forAllIndex(uGrid, ii) {
        dtPoint3 ttUV 
        = 
        thick->YdtPoint3Percent( analyticFunction::aFXTwoD( uGrid[ii][jj] ) );
        float tt = ttUV.x();
        aFX uvT = analyticFunction::aFXTwoD(ttUV.y(), ttUV.z());

        dtPoint3 yy = cV3d->YdtPoint3Percent(uvT);
        std::vector< dtVector3 > der = cV3d->DYdtVector3( *cV3d % uvT );		
        dtVector3 nn = cV3d->unitNdtVector3( *cV3d % uvT );		

        //
        // solve AA * xx = bb
        //
        dtMatrix AA
        = 
        dtLinearAlgebra::createMatrixGiveColumns(
          ::boost::assign::list_of
            (der[0])(der[1])( - nn + dtLinearAlgebra::dotProduct(nn, nf)*nf )
        );
        dtMatrixVector bb
        = 
        dtLinearAlgebra::createMatrixVector(-tt * nn);        
        dtMatrixVector xx = dtLinearAlgebra::solveMatrix(AA, bb);

        grid[ii][jj] = yy + tt * nn + xx[0] * der[0] + xx[1] * der[1];
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
    cV3d.reset( 
      new vec3dSurfaceTwoD(
        dt__tmpPtr(
          dtSurface, 
          bSplineSurface_skinConstructOCC(
            cL, skinOrderMin, skinOrderMax, skinNIterations
          ).result()
        )
      )      
    );
    
    _vS.reset( cV3d->clone() );
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
