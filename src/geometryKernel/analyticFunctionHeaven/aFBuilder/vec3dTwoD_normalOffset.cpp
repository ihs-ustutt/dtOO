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
    int const & skinNIterations,
    bool const & closeU,
    float const & closeSmooth
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
    if (closeSmooth > 0.) {
      dt__forAllIndex(uGrid[0], jj) {        
        dtPoint3 p_s = grid[0][jj];
        dtPoint3 p_sNext = grid[1][jj];
        dtPoint3 p_ePrev = grid[grid.size(0)-2][jj];
        dtPoint3 p_e = grid[grid.size(0)-1][jj];

        dtVector3 ddU_s = p_sNext - p_s;
        dtVector3 ddU_e = p_e - p_ePrev;
        dtVector3 ddU_mean = 0.5 * (ddU_s + ddU_e);
        
        grid[1][jj] = grid[0][jj] + closeSmooth * ddU_mean;
        grid[grid.size(0)-2][jj] 
        = 
        grid[grid.size(0)-1][jj] - closeSmooth * ddU_mean;
        dt__info(
          vec3dTwoD_normalOffset(),
          << logMe::dtFormat(
            "ddU start / ddU end [%d]: (%f, %f, %f)/(%f, %f, %f)\n"
            "ddU mean [%d]: (%f, %f, %f)\n"
            "startNext / lastPrev [%d]:  (%f, %f, %f) / (%f, %f, %f) "
            "-> (%f, %f, %f) / (%f, %f, %f)"
          )
          % jj
          % ddU_s.x() % ddU_s.y() % ddU_s.z() 
          % ddU_e.x() % ddU_e.y() % ddU_e.z()
          % jj
          % ddU_mean.x() % ddU_mean.y() % ddU_mean.z()
          % jj
          % p_sNext.x() % p_sNext.y() % p_sNext.z()
          % p_ePrev.x() % p_ePrev.y() % p_ePrev.z()
          % grid[1][jj].x() % grid[1][jj].y() % grid[1][jj].z()
          % grid[grid.size(0)-2][jj].x() 
          % grid[grid.size(0)-2][jj].y() 
          % grid[grid.size(0)-2][jj].z()
        );         
      }
    }    
    if (closeU) {
      dt__forAllIndex(uGrid[0], jj) {        
        dtPoint3 p_s = grid[0][jj];
        dtPoint3 p_e = grid[grid.size(0)-1][jj];
        grid[0][jj] = p_e + 0.5*(p_s - p_e);
        grid[grid.size(0)-1][jj] = grid[0][jj];
        dt__info(
          vec3dTwoD_normalOffset(),
          << logMe::dtFormat(
            "Move start / end point [%d]: (%f, %f, %f)/(%f, %f, %f) "
            "-> (%f, %f, %f)"
          )
          % jj
          % p_s.x() % p_s.y() % p_s.z() % p_e.x() % p_e.y() % p_e.z()
          % grid[0][jj].x() % grid[0][jj].y() % grid[0][jj].z()
        );
      }
    }    
    dt__forFromToIndex(0, grid.size(1), jj) {
      cL[jj] = bSplineCurve_pointConstructOCC(grid.fixJ(jj), order).result();
      _vC.push_back( 
        new vec3dCurveOneD( 
          dt__tmpPtr(
            dtCurve, bSplineCurve_pointConstructOCC(
              grid.fixJ(jj), order
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
