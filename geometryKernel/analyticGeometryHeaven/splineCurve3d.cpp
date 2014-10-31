#include "splineCurve3d.h"
#include <logMe/logMe.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
#include <geometryEngine/dtCurve.h>
#include <progHelper.h>

namespace dtOO {
  splineCurve3d::splineCurve3d() : map1dTo3d() {
  }

  splineCurve3d::splineCurve3d(splineCurve3d const & orig) : map1dTo3d(orig){
    _dtC.reset( orig._dtC->clone() );
  }
  
  splineCurve3d::splineCurve3d(dtCurve const & orig) : map1dTo3d() {
    _dtC.reset( orig.clone() );
  }  

  splineCurve3d::splineCurve3d(dtCurve * orig) : map1dTo3d() {
    _dtC.reset( orig->clone());
  }
	
  splineCurve3d * splineCurve3d::clone( void ) const {
    return new splineCurve3d( *this );
  }

  splineCurve3d * splineCurve3d::create( void ) const {
    return new splineCurve3d();
  }

  splineCurve3d::~splineCurve3d() {
  }

  void splineCurve3d::dump(void) const {
    analyticGeometry::dump();
    
    if (_dtC) {
      _dtC->dump();
    }
    else {
      DTWARNINGWF(dump(),
              << "No SISL curve!" << LOGDEL
              << DTLOGEVAL(_dtC) );
    }
  }

  dtPoint3 splineCurve3d::getPoint(float const & uu) const {
    return (_dtC->getPoint3d( uu ));
  }
	
  vectorHandling< renderInterface * > splineCurve3d::getExtRender( void ) const {
		vectorHandling< renderInterface * > retVec;		
		retVec.push_back( new vectorContainer() );
    vectorContainer & vecCon = *(static_cast< vectorContainer * >(retVec.back()));
	  retVec.push_back( new pointContainer() );
    pointContainer & controlPointContainer = *(static_cast< pointContainer * >(retVec.back()));
		
    //
    // get spline direction
    //
    dtPoint3 startPoint = getPointPercent(0.05);
    dtPoint3 topPoint = getPointPercent(0.1);
    dtVector3 uu = topPoint - startPoint;

    //
    // add direction vector to container
    //    
    vecCon.add(uu, "", startPoint);

    //
    // put control points to pointContainer
    //
    int numPoints = _dtC->getNControlPoints();
    dtPoint3 tmpControlPoint;

    for (int ii=0; ii<numPoints; ii++) {
      tmpControlPoint = _dtC->getControlPoint3d(ii);
      controlPointContainer.add( tmpControlPoint, "" );          
    }
		return retVec;
  }

  dtCurve const * const splineCurve3d::ptrConstDtCurve(void) const {
    return _dtC.get();
  }
  
  void splineCurve3d::rotate(dtPoint3 const origin, dtVector3 const vector, float const angle) {
    _dtC->rotate(origin, vector, angle);
  }

  void splineCurve3d::revert( void ) {
    _dtC->revert();
  }
  
  void splineCurve3d::closeArithmetic(void) {
    _dtC->closeArithmetic();
  }

  void splineCurve3d::closeStraight(void) {
    _dtC->closeStraight();
  }  
  
  dtVector3 splineCurve3d::firstDerU( float const uu) const {
    return _dtC->firstDer(uu);
  }
  
  bool splineCurve3d::isClosed( int const & dir) const {
    return _dtC->closed();
  }
  
  float splineCurve3d::getMin( int const & dir) const {
    return _dtC->getMin(dir);
  }

  float splineCurve3d::getMax( int const & dir) const {
    return _dtC->getMax(dir);
  }  
}