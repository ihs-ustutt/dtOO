/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "vec3dTransVolThreeD_skinBSplineSurfaces.h"

#include <logMe/dtMacros.h>
#include <interfaceHeaven/vectorHandling.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <analyticFunctionHeaven/vec3dThreeD.h>
#include <analyticFunctionHeaven/vec3dTransVolThreeD.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>

namespace dtOO {
  vec3dTransVolThreeD_skinBSplineSurfaces::vec3dTransVolThreeD_skinBSplineSurfaces(
    vectorHandling< analyticFunction const * > const * const aF
  ) {
    vectorHandling< dtSurface const * > sL;
    dt__forAllRefAuto(*aF, anAF) {
      sL.push_back(
        vec3dSurfaceTwoD::MustConstDownCast(anAF)->constPtrDtSurface()
      );
    }
    vectorHandling< dtCurve const * > cL0;
    vectorHandling< dtCurve const * > cL1;
    vectorHandling< dtCurve const * > cL2;
    vectorHandling< dtCurve const * > cL3;
    dt__forAllIndex(sL, ii) {
      cL0.push_back( sL[ii]->segmentConstVPercent(0., 0., 1.) );
      cL1.push_back( sL[ii]->segmentConstUPercent(1., 0., 1.) );
      cL2.push_back( sL[ii]->segmentConstVPercent(1., 0., 1.) );
      cL3.push_back( sL[ii]->segmentConstUPercent(0., 0., 1.) );
    }
    ptrHandling< dtSurface > sL0(
      bSplineSurface_skinConstructOCC(cL0).result() 
    );
    ptrHandling< dtSurface > sL1(
      bSplineSurface_skinConstructOCC(cL1).result() 
    );
    ptrHandling< dtSurface > sL2(
      bSplineSurface_skinConstructOCC(cL2).result() 
    );
    ptrHandling< dtSurface > sL3(
      bSplineSurface_skinConstructOCC(cL3).result() 
    );			

    ptrHandling<vec3dSurfaceTwoD> v3d2d0(new vec3dSurfaceTwoD(sL.back()));
    ptrHandling<vec3dSurfaceTwoD> v3d2d1(new vec3dSurfaceTwoD(sL3.get()));
    ptrHandling<vec3dSurfaceTwoD> v3d2d2(new vec3dSurfaceTwoD(sL.front()));
    ptrHandling<vec3dSurfaceTwoD> v3d2d3(new vec3dSurfaceTwoD(sL1.get()));
    ptrHandling<vec3dSurfaceTwoD> v3d2d4(new vec3dSurfaceTwoD(sL0.get()));
    ptrHandling<vec3dSurfaceTwoD> v3d2d5(new vec3dSurfaceTwoD(sL2.get()));

    //
    // create vec3dSurfaceTwoD
    //
    _vT.reset(
      new vec3dTransVolThreeD( 
        v3d2d0.get(), v3d2d1.get(), v3d2d2.get(), 
        v3d2d3.get(), v3d2d4.get(), v3d2d5.get()
      )
    );
  }

  vec3dTransVolThreeD_skinBSplineSurfaces::~vec3dTransVolThreeD_skinBSplineSurfaces() {
  }

  vec3dTransVolThreeD * vec3dTransVolThreeD_skinBSplineSurfaces::result(void) const {
    return _vT->clone();
  }
}
