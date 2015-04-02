#include "spline3dToBlendedSurfaceBuilder.h"

#include <logMe/logMe.h>
#include <analyticGeometryHeaven/splineSurface3d.h>

#define SUCCESS 0
#define FAILURE 1

namespace dtOO {
  DTCLASSLOGMETHODI(spline3dToBlendedSurfaceBuilder,
          << dt__eval( thisRef._splineSurface3dP ) );
  
  spline3dToBlendedSurfaceBuilder::spline3dToBlendedSurfaceBuilder() {
    _splineSurface3dP = NULL;      
  }

  spline3dToBlendedSurfaceBuilder::~spline3dToBlendedSurfaceBuilder() {
    if (_splineSurface3dP) {
      delete _splineSurface3dP;
    }      
  }
  
  int spline3dToBlendedSurfaceBuilder::buildPart(void) {
    //
    // check input
    //
    if (_analyticGeometryP.getNAttributes() == 0) {
      dt__THROW( buildPart(),
              << dt__eval( _analyticGeometryP.getNAttributes() ) );
    }


    //create curve array
    int nCurves = _analyticGeometryP.getNAttributes();
    vectorHandling< analyticGeometry const * > myCurveP;
    for ( int ii=0;ii<nCurves;ii++ ) {
      myCurveP.push_back( *(_analyticGeometryP.getAttribute(ii+1)) );
    }

    _splineSurface3dP = new splineSurface3d();
    _splineSurface3dP->setConstruction("blended");
    _splineSurface3dP->createMe(&myCurveP);

    return SUCCESS;  
  }  
  
  void spline3dToBlendedSurfaceBuilder::final(void) {
    analyticGeometryBuilder::final();
    delete _splineSurface3dP;
  }

  void spline3dToBlendedSurfaceBuilder::getResult(vectorHandling<analyticGeometry*>* aGeo) {  
    aGeo->push_back( _splineSurface3dP->clone() );
  }
  
}
