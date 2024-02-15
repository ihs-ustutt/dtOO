/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "vec3dCurve2dInSurfaceOneD.h"
#include "dtLinearAlgebra.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtSurface.h>
#include <solid2dLine.h>
#include <discrete2dPoints.h>
#include <interfaceHeaven/staticPropertiesHandler.h>

#include "analyticFunctionTransformed.h"

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
	
	aFY vec3dCurve2dInSurfaceOneD::Y( aFX const & xx ) const {
		dtPoint2 pX = _dtC2d->point(xx[0]);
		dtPoint3 pY = _dtS->getPoint3d(pX);
		
		aFY yy(3, 0.);
		
		yy[0] = pY.x();
		yy[1] = pY.y();
		yy[2] = pY.z();
		
		return yy;
	}

  vec3dCurve2dInSurfaceOneD * vec3dCurve2dInSurfaceOneD::clone( void ) const {
		return new vec3dCurve2dInSurfaceOneD(*this);
	}
  
	vec3dCurve2dInSurfaceOneD * vec3dCurve2dInSurfaceOneD::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new analyticFunctionTransformed< vec3dCurve2dInSurfaceOneD >(*this, dtT);
	}    
	
  vec3dCurve2dInSurfaceOneD * vec3dCurve2dInSurfaceOneD::create( void ) const {
		return new vec3dCurve2dInSurfaceOneD();
	}
	
  vectorHandling< renderInterface * > vec3dCurve2dInSurfaceOneD::getRender( void ) const {
		int nU
		=
		staticPropertiesHandler::getInstance()->getOptionInt(
      "function_render_resolution_u"
    );	
		
		vectorHandling< dtPoint2 > p2(nU);
    dtReal interval = (xMax(0) - xMin(0)) / (nU-1);
    for (int ii=0;ii<nU;ii++) {
			dtReal iiF = static_cast<dtReal>(ii);
      dtReal xx = xMin(0) + iiF * interval;
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
	bool vec3dCurve2dInSurfaceOneD::closed( dtInt const & dir ) const {
		return _dtC2d->closed();
	}
}
