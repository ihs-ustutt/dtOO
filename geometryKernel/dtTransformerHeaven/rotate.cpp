#include "rotate.h"
#include "baseContainerHeaven/baseContainer.h"
#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/ptrHandling.h>

#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map2dTo3dTransformed.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>
#include <baseContainerHeaven/baseContainer.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>
#include <QtXml/QDomElement>

namespace dtOO {
  rotate::rotate() : dtTransformer() {
  }

  rotate::~rotate() {
  }
	
	rotate::rotate(const rotate& orig) : dtTransformer(orig) {
		_angle = orig._angle;
		_origin = orig._origin;
		_rotVector = orig._rotVector;
	}
	
  dtTransformer * rotate::clone( void ) const {
	  return new rotate(*this);	
	}
	
  dtTransformer * rotate::create( void ) const {
		return new rotate();
	}	

	std::vector< dtPoint3 > rotate::apply( std::vector< dtPoint3 > const * const toTrans ) const {
		dtAffTransformation3 rot = dtLinearAlgebra::getRotation(_rotVector, _angle);
	
		std::vector< dtPoint3 > ret(toTrans->size());
		dt__FORALL(*toTrans, ii,
			dtVector3 vv = rot.transform( toTrans->at(ii) - _origin );
		  ret[ii] = _origin + vv;
		);
		
		return ret;
	}
	
  vectorHandling< analyticGeometry * > rotate::apply( 
	  vectorHandling< analyticGeometry * > const * const aGeoVecP 
	) const {
    vectorHandling< analyticGeometry * > retAGeo;

    //each analyticGeometry
    for (int ii=0; ii< aGeoVecP->size(); ii++) {
			//
			// clone and cast analyticGeometry
			//
			dt__pH(analyticGeometry) aGeoP(aGeoVecP->at(ii)->clone());
			map2dTo3d * m2d = map2dTo3d::DownCast(aGeoP.get());
			analyticSurface * aS = analyticSurface::DownCast(aGeoP.get());
			map2dTo3dTransformed< analyticSurface > * aST 
			= 
			map2dTo3dTransformed< analyticSurface >::DownCast(aGeoP.get());
			if ( aS && !aST ) {
				ptrHandling< dtSurface > dtS(
					geomSurface_surfaceRotateConstructOCC( 
						aS->ptrDtSurface(), 
						_origin, 
						_rotVector, 
						_angle 
					).result()
				);
				aGeoP.reset( new analyticSurface(dtS.get()) );
			}
			else {
				aGeoP.reset( m2d->cloneTransformed(this) );
			}
			//
			// push translated geometry in vector
			//      
			retAGeo.push_back( aGeoP->clone() );
    }
    return retAGeo;
  }

  bool rotate::isNecessary( void ) const {
    return true;
  }

  void rotate::init( 
	  QDomElement const * tE, 
    baseContainer const * const bC,   
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    dtTransformer::init(tE, bC, cValP, sFunP, depAGeoP);
		
    if (dtXmlParserBase::hasAttribute("origin", *tE)) {
      _origin 
			= 
			dtXmlParserBase::getDtPoint3(dtXmlParserBase::getAttributeStr("origin", *tE), bC );
    }
    if (dtXmlParserBase::hasAttribute("rotation_vector", *tE)) {
      _rotVector
			= 
			dtXmlParserBase::getDtVector3(
				dtXmlParserBase::getAttributeStr("rotation_vector", *tE), 
				bC 
			);
    }
    if (dtXmlParserBase::hasAttribute("angle", *tE)) {
      _angle 
			= 
			dtXmlParserBase::getAttributeFloatMuParse(
		    "angle", 
				*tE, 
				cValP, 
				sFunP
			);
    }
    DTDEBUGWF(
			init(),
      << DTLOGPOI3D(_origin) << LOGDEL
      << DTLOGVEC3D(_rotVector) << LOGDEL
      << DTLOGEVAL(_angle) 
		);
  }
}