#include "rotSplineBuilder.h"

#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <analyticGeometryHeaven/splineCurve3d.h>

namespace dtOO {
  DTCLASSLOGMETHODI(rotSplineBuilder,
          << dt__eval( thisRef.myRotBSpline ) );

  rotSplineBuilder::rotSplineBuilder() {

  }

  rotSplineBuilder::~rotSplineBuilder() {
    delete myRotBSpline;
  }

  int rotSplineBuilder::buildPart(void) { 
    bool hasVector = false;
    bool hasPoint = false;
    bool hasAGeo = false;
    bool hasAngle = false;
    
    if (_vector.getNAttributes() == 1) {
      hasVector = true;
    }
    if (_point.getNAttributes() == 1) {
      hasPoint = true;
    }
    if (_analyticGeometryP.getNAttributes() == 1) {
      hasAGeo = true;
    }    
    if (_float.getNAttributes() == 1) {
      hasAngle = true;
    }        
//    //some checks
//    if (_vector.getNAttributes() != 1) {
//      dt__THROW( buildPart(),
//              << dt__eval( _vector.getNAttributes() ) );
//    }
//    if (_analyticGeometryP.getNAttributes() != 1) {
//      dt__THROW( buildPart(),
//              << dt__eval( _analyticGeometryP.getNAttributes() ) );
//    }
//    else {
//      splineCurve3dP = dynamic_cast<splineCurve3d*> ( *(_analyticGeometryP.getAttribute(1)) );
//    }
//    if (_point.getNAttributes() != 1) {
//      dt__THROW( buildPart(),
//              << dt__eval( _point.getNAttributes() ) );
//    }  
    if ( hasAGeo && hasPoint && hasVector ) {
      dtPoint3 origin =  *( _point.getAttribute(1) );
      dtVector3 axis = *( _vector.getAttribute(1) );

      splineCurve3d * sC3dP = dynamic_cast< splineCurve3d * >( 
                                *(_analyticGeometryP.getAttribute(1)) 
                              );
      
      //
      // check origin and move if necessary
      //
      dtPoint3 start = sC3dP->getPointPercent(0.);
      dtVector3 vv = start - origin;
      if ( vv*axis != 0. ) {
        float adjusting = (vv*axis)/sqrt(axis.squared_length());
        
        origin = origin + axis  * adjusting;
        
        dt__warning(buildPart(),
                << dt__eval(vv*axis) << std::endl 
                << "Origin of rotSpline is not correct!" << std::endl
                << "Move origin to " << dt__point3d(origin) );        
      }
      
      if ( hasAngle ) {
        float & angle = _float.getAttributeRef(0);
        myRotBSpline = new rotatingSpline(sC3dP, origin, axis, angle);
      }
      else {
        myRotBSpline = new rotatingSpline(sC3dP, origin, axis);
      }
    }
  }

  void rotSplineBuilder::getResult(vectorHandling< analyticGeometry* >* aGeo) {
    aGeo->push_back( myRotBSpline->clone() );
  }

  void rotSplineBuilder::final(void) {
    analyticGeometryBuilder::final();
    delete myRotBSpline;
  }
}
