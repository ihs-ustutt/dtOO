#include "scaSISLSplineTwoDBuilder.h"
#include <interfaceHeaven/ptrHandling.h>
#include <functionHeaven/scaCurve2dOneD.h>
#include <geometryEngine/dtSislCurve2d.h>
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
  scaSISLSplineTwoDBuilder::scaSISLSplineTwoDBuilder() {
    _scaSISLSplineTwoDP = NULL;
  }

  scaSISLSplineTwoDBuilder::~scaSISLSplineTwoDBuilder() {
    delete _scaSISLSplineTwoDP;
  }

  int scaSISLSplineTwoDBuilder::buildPart(void) {
    /* ------------------------------------------------------------------------ */
    /* check input */
    /* ------------------------------------------------------------------------ */
    bool hasPoints = false;
    bool hasOrder = false;
    bool hasScaFunction = false;
    bool hasShapeFactor = false;
    if (_pointTwo.getNAttributes() > 0) {
      hasPoints = true;
    }
    if (_integer.getNAttributes() == 1) {
      hasOrder = true;
    }
    if (_float.getNAttributes() == 1) {
      hasShapeFactor = true;
    }    
    if (_scaFunctionP.getNAttributes() == 1) {
      hasScaFunction = true;
    }

    /* ------------------------------------------------------------------------ */
    // create spline out of points and an order
    /* ------------------------------------------------------------------------ */
    if ( hasPoints && hasOrder ) {
  //    double* pointsArray;
      int * pointType;
      int counter;
      int nPoints = _pointTwo.getNAttributes();
      int splineOrder = *(_integer.getAttribute(1));
//      dtCurve* SISLCurveP;

      //
      // array allocation and initialization
      //
      std::vector< dtPoint2 > pointsArray;// = new double[nPoints * dimension];
//      pointType = new int[nPoints];
      counter = 0;
      nPoints = _pointTwo.getNAttributes();
      for (int ii=0;ii<nPoints;ii++) {
        pointsArray.push_back( *(_pointTwo.getAttribute(ii+1)) );
      }

      ptrHandling<dtCurve2d> SISLCurveP(
			  new dtSislCurve2d(&pointsArray, splineOrder)
			);

      //
      // create scaSISLSplineTwoD
      //
      _scaSISLSplineTwoDP =  new scaSISLSplineTwoD( SISLCurveP.get() );
    }
    /* ------------------------------------------------------------------------ */
    // create conic arc spline out of points and a shape factor
    /* ------------------------------------------------------------------------ */
    else if ( hasPoints && hasShapeFactor ) {
      int* pointType;
      int counter;
      int nPoints = _pointTwo.getNAttributes();
      float shapeFactor = *(_float.getAttribute(1));

      //
      // array allocation and initialization
      //
      std::vector< dtPoint2 > pointsArray;// = new double[nPoints * dimension];
      pointType = new int[nPoints];
      counter = 0;
      nPoints = _pointTwo.getNAttributes();
      for (int ii=0;ii<nPoints;ii++) {
        pointsArray.push_back( *(_pointTwo.getAttribute(ii+1)) );
      }

      ptrHandling<dtCurve2d> SISLCurveP(
			  new dtSislCurve2d("conicArc", &pointsArray, shapeFactor)
			);

      //
      // create scaSISLSplineTwoD
      //
      _scaSISLSplineTwoDP =  new scaSISLSplineTwoD( SISLCurveP.get() );
    }    
    //
    // copy an existing scaSISLSplineTwoD
    //
    else if (hasScaFunction) {
      analyticFunction* const * const sFunP = _scaFunctionP.getAttribute(1);
      //
      // check cast
      //
      scaSISLSplineTwoD * sSISLS2dP;
      dt__MUSTDOWNCASTWM( 
        *sFunP, 
        scaSISLSplineTwoD, 
        sSISLS2dP,
        << "Object to copy is not a scaSISLSplineTwoD."
      );
      
      //
      // copy
      //
      _scaSISLSplineTwoDP = new scaSISLSplineTwoD( *sSISLS2dP );
    }
    else {
        dt__THROW(buildPart(),
                << DTLOGEVAL(hasOrder) << LOGDEL
                << DTLOGEVAL(hasScaFunction) << LOGDEL
                << DTLOGEVAL(hasPoints) );
    }
  }

  int scaSISLSplineTwoDBuilder::getResult(vectorHandling<analyticFunction*>* sFunP) {
    sFunP->push_back( _scaSISLSplineTwoDP->clone() );
  }
}