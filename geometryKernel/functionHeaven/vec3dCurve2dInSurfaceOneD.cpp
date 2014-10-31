#include "vec3dCurve2dInSurfaceOneD.h"
#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>

namespace dtOO {
	vec3dCurve2dInSurfaceOneD::vec3dCurve2dInSurfaceOneD() : vec3dOneD() {
	}

	vec3dCurve2dInSurfaceOneD::vec3dCurve2dInSurfaceOneD(const vec3dCurve2dInSurfaceOneD& orig) : vec3dOneD(orig) {
		_dtC2d.reset( orig._dtC2d->clone() );
		_dtS.reset( orig._dtS->clone() );
		setMin( _dtC2d->minU() );
		setMax( _dtC2d->maxU() );		
	}

	vec3dCurve2dInSurfaceOneD::~vec3dCurve2dInSurfaceOneD() {
	}
	
	vec3dCurve2dInSurfaceOneD::vec3dCurve2dInSurfaceOneD(dtCurve2d const * const dtC2d, dtSurface const * const dtS) : vec3dOneD() {
		_dtC2d.reset( dtC2d->clone() );
		_dtS.reset( dtS->clone() );
		setMin( _dtC2d->minU() );
		setMax( _dtC2d->maxU() );
	}
	
	aFY vec3dCurve2dInSurfaceOneD::Y( float const & xx) const {
		dtPoint2 pX = _dtC2d->point(xx);
		dtPoint3 pY = _dtS->getPoint3d(pX);
		
		aFY yy(3);
		
		yy[0] = pY.x();
		yy[1] = pY.y();
		yy[2] = pY.z();
		
		return yy;
	}

  vec3dCurve2dInSurfaceOneD * vec3dCurve2dInSurfaceOneD::clone( void ) const {
		return new vec3dCurve2dInSurfaceOneD(*this);
	}
	
  vec3dCurve2dInSurfaceOneD * vec3dCurve2dInSurfaceOneD::create( void ) const {
		return new vec3dCurve2dInSurfaceOneD();
	}
	
  vectorHandling< renderInterface * > vec3dCurve2dInSurfaceOneD::getRender( void ) const {
		vectorHandling< dtPoint2 > p2(_nVis);
    float interval = (xMax(0) - xMin(0)) / (_nVis-1);
    for (int ii=0;ii<_nVis;ii++) {
			float iiF = static_cast<float>(ii);
      float xx = xMin(0) + iiF * interval;
			dtPoint3 p3 = YdtPoint3(xx);
      p2[ii] = dtPoint2(p3.x(), p3.y());
    }
		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = new solid2dLine(p2);
		
		return rV;
  }	
	
	/**
	 * 
   * @todo: What if surface is closed and curve goes in periodic direction?
   */
	bool vec3dCurve2dInSurfaceOneD::closed( int const & dir ) const {
		return _dtC2d->closed();
	}
}
