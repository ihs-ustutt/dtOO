#include "splineCurve3d.h"
#include <logMe/logMe.h>
#include <discrete3dVector.h>
#include <discrete3dPoints.h>
#include <geometryEngine/dtCurve.h>
#include <logMe/dtMacros.h>
#include "map1dTo3dTransformed.h"
#include <dtTransformerHeaven/dtTransformer.h>

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

	splineCurve3d * splineCurve3d::cloneTransformed( 
    dtTransformer const * const dtT 
  ) const {
		return new map1dTo3dTransformed<splineCurve3d>(*this, dtT);
	}
  
  splineCurve3d * splineCurve3d::create( void ) const {
    return new splineCurve3d();
  }

  splineCurve3d::~splineCurve3d() {
  }

  dtPoint3 splineCurve3d::getPoint(float const & uu) const {
    return (_dtC->point( uu ));
  }
	
  vectorHandling< renderInterface * > splineCurve3d::getExtRender( 
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
      int numPoints = _dtC->nControlPoints();
			dtPoint3 tmpControlPoint;

			for (int ii=0; ii<numPoints; ii++) {
				tmpControlPoint = _dtC->controlPoint(ii);
				pp.push_back(tmpControlPoint);
			}
			retVec.push_back( new discrete3dPoints(pp) );
		}
		
		return retVec;
  }

  dtCurve const * const splineCurve3d::ptrConstDtCurve(void) const {
    return _dtC.get();
  }
  
  dtVector3 splineCurve3d::firstDerU( float const uu) const {
    return _dtC->firstDer(uu);
  }

	float splineCurve3d::l_u( float const & uu ) const {
		return _dtC->l_u(uu);	
	}	
  
  float splineCurve3d::u_l( float const & ll ) const {
    return _dtC->u_l(ll);
  } 
  
  bool splineCurve3d::isClosed( int const & dir) const {
    return _dtC->closed();
  }
  
  float splineCurve3d::getMin( int const & dir) const {
    return _dtC->minPara(dir);
  }

  float splineCurve3d::getMax( int const & dir) const {
    return _dtC->maxPara(dir);
  }  

	std::string splineCurve3d::dumpToString( void ) const {
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