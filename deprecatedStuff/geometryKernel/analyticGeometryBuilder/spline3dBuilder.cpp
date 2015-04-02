#include "spline3dBuilder.h"

#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <progHelper.h>
#include <logMe/logMe.h>

#include <logMe/logMe.h>

namespace dtOO {
  DTCLASSLOGMETHODI(spline3dBuilder,
          << dt__eval( thisRef._splineCurve ) );

  spline3dBuilder::spline3dBuilder() {
    _splineCurve = NULL;
  }

  spline3dBuilder::~spline3dBuilder() {
    delete _splineCurve;
  }

  int spline3dBuilder::buildPart(void) {
    /* ------------------------------------------------------------------------ */
    /* check input */
    /* ------------------------------------------------------------------------ */
    bool hasPoints = true;
    bool hasOrder = true;
    bool hasSpline = true;
    if (_point.getNAttributes() == 0) {
      hasPoints = false;
    }
    if (_integer.getNAttributes() != 1) {
      hasOrder = false;
    }
    if (_analyticGeometryP.getNAttributes() <= 0) {
      hasSpline = false;
    }

    //
    //create new spline
    //
    if ( hasPoints && hasOrder ) {
      std::vector< dtPoint3 > splinePoints;
      for (int ii=1; ii<=_point.getNAttributes(); ii++) {
        splinePoints.push_back( 
          dtPoint3( 
            *(_point.getAttribute(ii)) 
          ) 
        );        
      }
      int splineOrder = *(_integer.getAttribute(1));
      _splineCurve = new splineCurve3d( &splinePoints, splineOrder );
    }
    //
    // copy spline
    //
    else if (hasSpline) {
      //
      // put all splineCurve3d in a vector
      //
      vectorHandling< splineCurve3d * > s3CVec;
      for (int ii=1;ii<=_analyticGeometryP.getNAttributes();ii++) {
        splineCurve3d * sC3;
        dt__mustCast(
          *(_analyticGeometryP.getAttribute(ii)), 
          splineCurve3d, 
          sC3
        );
        s3CVec.push_back( sC3 );        
      }
      //
      // create new splineCurve3d
      //  
      if (s3CVec.size() == 1) {
        _splineCurve = s3CVec[0]->clone();//splineCurve3d( *(s3CVec[0]) );
      }
      else {
        _splineCurve = new splineCurve3d( s3CVec );
      }
    }
    //
    // error
    //
    else {
      dt__THROW(buildPart(),
              << dt__eval(hasPoints) << std::endl
              << dt__eval(hasOrder) << std::endl
              << dt__eval(hasSpline) );
    }
  }

  void spline3dBuilder::getResult(vectorHandling< analyticGeometry* >* aGeo) {
    aGeo->push_back( _splineCurve->clone() );
  }

  void spline3dBuilder::final(void) {
    analyticGeometryBuilder::final();
    delete _splineCurve;
  }
}