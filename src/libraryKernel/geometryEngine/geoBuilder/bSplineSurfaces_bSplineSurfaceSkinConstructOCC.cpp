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

#include "bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h"
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/dtCurve.h>
#include "bSplineSurface_skinConstructOCC.h"

namespace dtOO {
  bSplineSurfaces_bSplineSurfaceSkinConstructOCC
    ::bSplineSurfaces_bSplineSurfaceSkinConstructOCC( 
	  vectorHandling< dtSurface const * > const & dtS
	) {
    vectorHandling< dtCurve const * > cL0;
    vectorHandling< dtCurve const * > cL1;
    vectorHandling< dtCurve const * > cL2;
    vectorHandling< dtCurve const * > cL3;
    dt__forAllIndex(dtS, ii) {
    	cL0.push_back( dtS[ii]->segmentConstVPercent(0., 0., 1.) );
      cL1.push_back( dtS[ii]->segmentConstUPercent(1., 0., 1.) );
      cL2.push_back( dtS[ii]->segmentConstVPercent(1., 0., 1.) );
      cL3.push_back( dtS[ii]->segmentConstUPercent(0., 0., 1.) );
    }
    dtSurface * sL0 = bSplineSurface_skinConstructOCC(cL0).result();
    dtSurface * sL1 = bSplineSurface_skinConstructOCC(cL1).result();
    dtSurface * sL2 = bSplineSurface_skinConstructOCC(cL2).result();
    dtSurface * sL3 = bSplineSurface_skinConstructOCC(cL3).result();			
    
    _dtS.resize(6);
    _dtS[0] = dtS.back()->clone();
    _dtS[1] = sL3;
    _dtS[2] = dtS.front()->clone();
    _dtS[3] = sL1;
    _dtS[4] = sL0;
    _dtS[5] = sL2;
	}

  bSplineSurfaces_bSplineSurfaceSkinConstructOCC
    ::bSplineSurfaces_bSplineSurfaceSkinConstructOCC(
    dtSurface const * const dtS0, dtSurface const * const dtS1
  ) {
    vectorHandling< dtSurface const * > vh;
    vh.push_back( dtS0 );
    vh.push_back( dtS1 );

    _dtS = bSplineSurfaces_bSplineSurfaceSkinConstructOCC(vh).result();
  }

	bSplineSurfaces_bSplineSurfaceSkinConstructOCC
    ::~bSplineSurfaces_bSplineSurfaceSkinConstructOCC() {
		
	}
	
	vectorHandling< dtSurface * > 
  bSplineSurfaces_bSplineSurfaceSkinConstructOCC::result( void ) {
		return _dtS;
	}
}
