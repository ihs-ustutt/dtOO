#include "reverse.h"
#include "geometryEngine/geoBuilder/geomCurve_curveReverseConstructOCC.h"
#include <progHelper.h>
#include <logMe/logMe.h>

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomCurve_curveReverseConstructOCC.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceReverseConstructOCC.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

namespace dtOO {
  reverse::reverse() : dtTransformer() {
    _revU = false;
    _revV = false;
    _revW = false;
  }

  reverse::~reverse() {
  }

	reverse::reverse(const reverse& orig) : dtTransformer(orig) {
		_revU = orig._revU;
    _revV = orig._revV;
    _revW = orig._revW;
	}  
  
  dtTransformer * reverse::clone( void ) const {
	  return new reverse(*this);
	}
	
  dtTransformer * reverse::create( void ) const {
		return new reverse();
	}
	
  aGPtrVec reverse::apply( 
    aGPtrVec const * const aGeoVecP 
  ) const {
    aGPtrVec retAGeo;

    dt__forAllConstIter(aGPtrVec, *aGeoVecP, it) {
			//
			// clone and cast analyticGeometry
			//
      map1dTo3d const * const m1d = map1dTo3d::ConstDownCast(*it);      
      map2dTo3d const * const m2d = map2dTo3d::ConstDownCast(*it);
      map3dTo3d const * const m3d = map3dTo3d::ConstDownCast(*it);
      
      if (m1d) {
        analyticCurve const * const s3 = analyticCurve::ConstDownCast(*it);
        if ( s3 &&  !s3->isTransformed() ) {
          retAGeo.push_back( 
            new analyticCurve(
              dt__tmpPtr(
                dtCurve,
                geomCurve_curveReverseConstructOCC( 
                  s3->ptrConstDtCurve(), _revU
                ).result()
              )
            ) 
          );
        }
        else {
          retAGeo.push_back( m1d->cloneTransformed(this) );
        }
      }
			if (m2d) {
        analyticSurface const * const aS = analyticSurface::ConstDownCast(*it);
        if ( aS &&  !aS->isTransformed() ) {
          retAGeo.push_back( 
            new analyticSurface(
              dt__tmpPtr(
                dtSurface,
                geomSurface_surfaceReverseConstructOCC( 
                  aS->ptrDtSurface(), _revU, _revV
                ).result()
              )
            ) 
          );
        }
        else dt__throwUnexpected(apply());
      }
//			else if (m3d) {
//        retAGeo.push_back( m3d->cloneTransformed(this) );
//      }
      else dt__throwUnexpected(apply());
    }
    return retAGeo;
  }

  bool reverse::isNecessary( void ) const {
    return true;
  }

  void reverse::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    if (dtXmlParserBase::hasAttribute("reverse_u", *tE)) {
      _revU = dtXmlParserBase::getAttributeBool("reverse_u", *tE);
    }
    if (dtXmlParserBase::hasAttribute("reverse_v", *tE)) {
      _revV = dtXmlParserBase::getAttributeBool("reverse_v", *tE);
    }
    if (dtXmlParserBase::hasAttribute("reverse_w", *tE)) {
      _revW = dtXmlParserBase::getAttributeBool("reverse_w", *tE);
    }    
  }
}