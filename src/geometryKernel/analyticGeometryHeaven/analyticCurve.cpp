#include "analyticCurve.h"
#include <logMe/logMe.h>
#include <discrete3dVector.h>
#include <discrete3dPoints.h>
#include <geometryEngine/dtCurve.h>
#include <logMe/dtMacros.h>
#include "map1dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>

namespace dtOO {
  analyticCurve::analyticCurve() : map1dTo3d() {
  }

  analyticCurve::analyticCurve(analyticCurve const & orig) : map1dTo3d(orig){
    _dtC.reset( orig._dtC->clone() );
  }
  
  analyticCurve::analyticCurve(dtCurve const & orig) : map1dTo3d() {
    _dtC.reset( orig.clone() );
  }  

  analyticCurve::analyticCurve(dtCurve * orig) : map1dTo3d() {
    _dtC.reset( orig->clone());
  }
	
  analyticCurve * analyticCurve::clone( void ) const {
    return new analyticCurve( *this );
  }

	analyticCurve * analyticCurve::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new map1dTo3dTransformed<analyticCurve>(*this, dtT);
	}
  
  analyticCurve * analyticCurve::create( void ) const {
    return new analyticCurve();
  }

  analyticCurve::~analyticCurve() {
  }

  dtPoint3 analyticCurve::getPoint(dtReal const & uu) const {
    return (_dtC->point( uu ));
  }
	
  map1dTo3d * analyticCurve::segment( 
    dtReal const & u0, dtReal const & u1 
  ) const {
    return new analyticCurve(
      dt__pH(dtCurve)(
        trimmedCurve_uBounds( _dtC.get(), u0, u1 ).result()
      ).get()
    );
//    scaLinearOneD fun(0, 1, u0, u1);
//    return new scaOneDInMap1dTo3d(&fun, this);
  }
  
  vectorHandling< renderInterface * > analyticCurve::getExtRender( 
    void 
  ) const {
		vectorHandling< dtVector3 > vv;
		vectorHandling< dtPoint3 > vp;
		vectorHandling< dtPoint3 > pp;
    //
    // get spline direction
    //
    dtPoint3 startPoint = getPointPercent(0.05);
    dtPoint3 topPoint = getPointPercent(0.1);
    dtVector3 uu = topPoint - startPoint;

    //
    // add direction vector to container
    //    
		vv.push_back(uu);
		vp.push_back(startPoint);

		vectorHandling< renderInterface * > retVec;
		retVec.push_back( new discrete3dVector(vv, vp) );

		//
		// control points
		//		
		if (_dtC->nControlPoints() != 0) {
      dtInt numPoints = _dtC->nControlPoints();
			dtPoint3 tmpControlPoint;

			for (int ii=0; ii<numPoints; ii++) {
				tmpControlPoint = _dtC->controlPoint(ii);
				pp.push_back(tmpControlPoint);
			}
			retVec.push_back( new discrete3dPoints(pp) );
		}
		
		return retVec;
  }

  dtCurve * analyticCurve::ptrDtCurve(void) const {
    return _dtC.get();
  }
  
  dtCurve const * const analyticCurve::ptrConstDtCurve(void) const {
    return _dtC.get();
  }
  
  dtVector3 analyticCurve::firstDerU( dtReal const uu) const {
    return _dtC->firstDer(uu);
  }

	dtReal analyticCurve::l_u( dtReal const & uu ) const {
		return _dtC->l_u(uu);	
	}	
  
  dtReal analyticCurve::u_l( dtReal const & ll ) const {
    return _dtC->u_l(ll);
  } 
  
  bool analyticCurve::isClosed( dtInt const & dir) const {
    return _dtC->closed();
  }
  
  dtReal analyticCurve::getMin( dtInt const & dir) const {
    return _dtC->minPara(dir);
  }

  dtReal analyticCurve::getMax( dtInt const & dir) const {
    return _dtC->maxPara(dir);
  }  

	std::string analyticCurve::dumpToString( void ) const {
		std::stringstream ss;
		
		ss
    << 
    dt__dumpToString( 
      << "_dtC = " << _dtC->virtualClassName() << std::endl
      << _dtC->dumpToString()
    );
		
		return ss.str();
	}	  
}