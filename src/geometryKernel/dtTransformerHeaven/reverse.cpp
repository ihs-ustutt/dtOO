#include "reverse.h"
#include <progHelper.h>
#include <logMe/logMe.h>

#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>

#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomCurve_curveReverseConstructOCC.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceReverseConstructOCC.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool reverse::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(reverse, new reverse())
  );
  
  reverse::reverse() : dtTransformer() {
  }

  reverse::~reverse() {
  }

	reverse::reverse(const reverse& orig) : dtTransformer(orig) {
	}  
  
  reverse::reverse( 
    jsonPrimitive const & jE 
  ) : dtTransformer(jE) {
    this->jInit(jE, NULL, NULL, NULL, NULL);
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
                  s3->ptrConstDtCurve(), config().lookupDef("_revU", false)
                ).result()
              )
            ) 
          );
        }
        else {
          retAGeo.push_back( m1d->cloneTransformed(this) );
        }
      }
			else if (m2d) {
        analyticSurface const * const aS = analyticSurface::ConstDownCast(*it);
        if ( aS &&  !aS->isTransformed() ) {
          retAGeo.push_back( 
            new analyticSurface(
              dt__tmpPtr(
                dtSurface,
                geomSurface_surfaceReverseConstructOCC( 
                  aS->ptrDtSurface(), 
                  config().lookupDef("_revU", false), 
                  config().lookupDef("_revV", false)
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

  aFPtrVec reverse::apply( aFPtrVec const * const aF ) const {
    aFPtrVec ret;

    dt__forAllRefAuto(*aF, theF) {
			//
			// clone and cast analyticGeometry
			//
      vec3dCurveOneD const * const v1d = vec3dCurveOneD::ConstDownCast(theF);      
      vec3dSurfaceTwoD const * const v2d = vec3dSurfaceTwoD::ConstDownCast(theF);      
      if (v1d) {
        ret.push_back( 
          new vec3dCurveOneD(
            dt__tmpPtr(
              dtCurve,
              geomCurve_curveReverseConstructOCC( 
                v1d->ptrConstDtCurve(), config().lookupDef("_revU", false)
              ).result()
            )
          ) 
        );
      }
			else if (v2d) {
        ret.push_back( 
          new vec3dSurfaceTwoD(
            dt__tmpPtr(
              dtSurface,
              geomSurface_surfaceReverseConstructOCC( 
                v2d->ptrDtSurface(), 
                config().lookupDef("_revU", false), 
                config().lookupDef("_revV", false)
              ).result()
            )
          ) 
        );
      }
      else dt__throwUnexpected(apply());
    }
    return ret;
  }  
  
  bool reverse::isNecessary( void ) const {
    return true;
  }

  void reverse::jInit( 
    jsonPrimitive const & jE,
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::jInit(jE, bC, cV, aF, aG);
  }  
  
  void reverse::init( 
	  ::QDomElement const * tE, 
    baseContainer * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    jsonPrimitive config;
    config.append(
      "_revU",
			dtXmlParserBase::getAttributeBool("reverse_u", *tE)
    );
    config.append(
      "_revV",
			dtXmlParserBase::getAttributeBool("reverse_v", *tE)
    );
    config.append(
      "_revW",
			dtXmlParserBase::getAttributeBool("reverse_w", *tE)
    );
    jInit(config, bC, cV, aF, aG);    
  }
}