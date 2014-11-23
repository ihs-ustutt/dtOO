#include "vec3dCurveOneD.h"
#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <geometryEngine/dtCurve.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	vec3dCurveOneD::vec3dCurveOneD() : vec3dOneD() {
	}

	vec3dCurveOneD::vec3dCurveOneD(const vec3dCurveOneD& orig) : vec3dOneD(orig) {
		_dtC.reset( orig._dtC->clone() );
	}

	vec3dCurveOneD::~vec3dCurveOneD() {
	}
	
	vec3dCurveOneD::vec3dCurveOneD(dtCurve const * const dtC) : vec3dOneD() {
		_dtC.reset( dtC->clone() );
		setMin( dtC->getUMin() );
		setMax( dtC->getUMax() );
	}
	
	aFY vec3dCurveOneD::Y( aFX const & xx ) const {
		dtPoint3 p2d = _dtC->getPoint3d(xx[0]);
		
		aFY yy(3);
		
		yy[0] = p2d.x();
		yy[1] = p2d.y();
		yy[2] = p2d.z();
		
		return yy;
	}

  vec3dCurveOneD * vec3dCurveOneD::clone( void ) const {
		return new vec3dCurveOneD(*this);
	}
	
  vec3dCurveOneD * vec3dCurveOneD::create( void ) const {
		return new vec3dCurveOneD();
	}
	
  vectorHandling< renderInterface * > vec3dCurveOneD::getRender( void ) const {
		int nU
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_u"
    );		
		
		vectorHandling< dtPoint2 > p2(nU);
    float interval = (xMax(0) - xMin(0)) / (nU-1);
    for (int ii=0;ii<nU;ii++) {
			float iiF = static_cast<float>(ii);
      float xx = xMin(0) + iiF * interval;
			dtPoint3 p3 = YdtPoint3(xx);
      p2[ii] = dtPoint2(p3.x(), p3.y());
    }
		
//		vectorHandling< dtPoint2 > p2Cp(_dtC->nControlPoints());
//    for (int ii=0;ii<p2Cp.size();ii++) {
//      p2Cp[ii] = _dtC->controlPoint( ii );
//    }
//		
		vectorHandling< renderInterface * > rV(1);
		rV[0] = new solid2dLine(p2);
//		rV[1] = new discrete2dPoints(p2Cp);
		
		return rV;
  }	
	
	dtCurve const * vec3dCurveOneD::ptrDtCurve( void ) const {
		return _dtC.get();
	}
	
	bool vec3dCurveOneD::closed( int const & dir ) const {
		return _dtC->closed();
	}
}
