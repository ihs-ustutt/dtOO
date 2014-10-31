#include "spline3dToSurfaceBuilder.h"

#define SUCCESS 0
#define FAILURE 1

namespace dtOO {
  DTCLASSLOGMETHODI(spline3dToSurfaceBuilder,
          << DTLOGEVAL( thisRef._splineSurface3dP ) );

  spline3dToSurfaceBuilder::spline3dToSurfaceBuilder() {
    _splineSurface3dP = NULL;  
  }

  spline3dToSurfaceBuilder::~spline3dToSurfaceBuilder() {
    if (_splineSurface3dP) {
      delete _splineSurface3dP;
    }  
  }

  int spline3dToSurfaceBuilder::buildPart(void) {
    vectorHandling< analyticGeometry const * > myCurveP;
    int loftedDirectionOrder;
    int nCurves;

    /*INPUT --------------------------------------------------------------------*/
    if (_analyticGeometryP.getNAttributes() == 0) {
      dt__THROW( buildPart(),
              << DTLOGEVAL( _analyticGeometryP.getNAttributes() ) );
    }
    if (_integer.getNAttributes() == 0) {
      dt__THROW( buildPart(),
              << DTLOGEVAL(_integer.getNAttributes() ) << LOGDEL
              << "Should be (1) loftedDirectionOrder");
      return FAILURE;
    }
    /*end INPUT ----------------------------------------------------------------*/

    //create curve array
    nCurves = _analyticGeometryP.getNAttributes();
    for ( int ii=0;ii<nCurves;ii++ ) {
      myCurveP.push_back( *(_analyticGeometryP.getAttribute(ii+1)) );
    }
    loftedDirectionOrder = *(_integer.getAttribute(1));

//    _splineSurface3dP = new splineSurface3d(&myCurveP, loftedDirectionOrder);
    _splineSurface3dP = new splineSurface3d();
    _splineSurface3dP->setLoftDirectionOrder(loftedDirectionOrder);
    _splineSurface3dP->createMe(&myCurveP);

    return SUCCESS;  
  }

  void spline3dToSurfaceBuilder::final(void) {
    analyticGeometryBuilder::final();
    delete _splineSurface3dP;
  }

  void spline3dToSurfaceBuilder::getResult(vectorHandling<analyticGeometry*>* aGeo) {  
    aGeo->push_back( _splineSurface3dP->clone() );
  }
}
