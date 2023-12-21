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
    ::~bSplineSurfaces_bSplineSurfaceSkinConstructOCC() {
		
	}
	
	vectorHandling< dtSurface * > 
  bSplineSurfaces_bSplineSurfaceSkinConstructOCC::result( void ) {
		return _dtS;
	}
}
