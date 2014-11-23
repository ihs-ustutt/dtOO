#include "vec2dCurve2dOneD.h"
#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include <progHelper.h>
#include <geometryEngine/dtCurve2d.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

namespace dtOO {
	vec2dCurve2dOneD::vec2dCurve2dOneD() : vec2dOneD() {
	}

	vec2dCurve2dOneD::vec2dCurve2dOneD(const vec2dCurve2dOneD& orig) : vec2dOneD(orig) {
		_dtC2d.reset( orig._dtC2d->clone() );
	}

	vec2dCurve2dOneD::~vec2dCurve2dOneD() {
	}
	
	vec2dCurve2dOneD::vec2dCurve2dOneD(dtCurve2d const * const dtC2d) : vec2dOneD() {
		_dtC2d.reset( dtC2d->clone() );
		setMin( dtC2d->minU() );
		setMax( dtC2d->maxU() );
	}
	
	aFY vec2dCurve2dOneD::Y( float const & xx) const {
		dtPoint2 p2d = _dtC2d->point(xx);
		
		aFY yy(2);
		
		yy[0] = p2d.x();
		yy[1] = p2d.y();
		
		return yy;
	}

  analyticFunction * vec2dCurve2dOneD::clone( void ) const {
		return new vec2dCurve2dOneD(*this);
	}
	
  analyticFunction * vec2dCurve2dOneD::create( void ) const {
		return new vec2dCurve2dOneD();
	}
	
  vectorHandling< renderInterface * > vec2dCurve2dOneD::getRender( void ) const {
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
      p2[ii] = YdtPoint2(xx);
    }
		
		vectorHandling< dtPoint2 > p2Cp(_dtC2d->nControlPoints());
    for (int ii=0;ii<p2Cp.size();ii++) {
      p2Cp[ii] = _dtC2d->controlPoint( ii );
    }
		
		vectorHandling< renderInterface * > rV(2);
		rV[0] = new solid2dLine(p2);
		rV[1] = new discrete2dPoints(p2Cp);
		
		return rV;
  }	
	
	dtCurve2d const * vec2dCurve2dOneD::ptrDtCurve2d( void ) const {
		return _dtC2d.get();
	}
}
