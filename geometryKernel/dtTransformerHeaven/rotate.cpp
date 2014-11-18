#include "rotate.h"
#include <logMe/logMe.h>
#include <geometryEngine/dtSurface.h>
#include <interfaceHeaven/ptrHandling.h>

#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/geoBuilder/geomSurface_surfaceRotateConstructOCC.h>

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
		_nPieces = orig._nPieces;
		_origin = orig._origin;
		_rotVector = orig._rotVector;
	}
	
  dtTransformer * rotate::clone( void ) const {
	  return new rotate(*this);	
	}
	
  dtTransformer * rotate::create( void ) const {
		return new rotate();
	}	

  vectorHandling< analyticGeometry * > rotate::apply( vectorHandling< analyticGeometry * > const * const aGeoVecP ) const {
    vectorHandling< analyticGeometry * > retAGeo;

    //each analyticGeometry
    for (int ii=0;ii< aGeoVecP->size();ii++) {
      //each piece
      for (int jj=1;jj<=_nPieces;jj++) {
        dt__TOFLOAT(float jjF, jj);

        //
        // clone and cast analyticGeometry
        //
        analyticGeometry * aGeoP = aGeoVecP->at(ii);//->clone();
        analyticSurface * aS = analyticSurface::DownCast(aGeoP);
        if ( aS ) {
					ptrHandling< dtSurface > dtS(
						geomSurface_surfaceRotateConstructOCC( 
							aS->ptrDtSurface(), 
							_origin, 
							_rotVector, 
							jjF*_angle 
						).result()
					);
					aGeoP = new analyticSurface( dtS.get() );
        }    
        else {
          DTWARNINGWF(apply(), << DTLOGEVAL(aS) );
        }      
        //
        // push translated geometry in vector
        //      
        retAGeo.push_back( aGeoP );
      }
    }
    return retAGeo;
  }

  bool rotate::isNecessary( void ) const {
    return true;
  }

  void rotate::init( QDomElement * transformerElementP, 
                     pointContainer * const pointContainerP,
                     vectorContainer * const vectorContainerP,    
                     vectorHandling< constValue * > const * const cValP,
                     vectorHandling< analyticFunction * > const * const sFunP,
                     vectorHandling< analyticGeometry * > const * const depAGeoP ) {

    if (transformerElementP->hasAttribute("origin")) {
      _origin = pointContainerP->get( getAttributeStr("origin", *transformerElementP) );
    }
    if (transformerElementP->hasAttribute("rotation_vector")) {
      _rotVector = vectorContainerP->get( getAttributeStr("rotation_vector", *transformerElementP) );
    }
    if (transformerElementP->hasAttribute("angle")) {
      _angle 
			= 
			getAttributeFloatMuParse(
		    "angle", 
				*transformerElementP, 
				cValP, 
				sFunP
			);
    }
    if (transformerElementP->hasAttribute("number_pieces")) {
      _nPieces
			= 
			getAttributeIntMuParse(
		    "number_pieces", 
				*transformerElementP, 
				cValP, 
				sFunP
			);
    }  
    DTDEBUGWF(init(),
            << DTLOGPOI3D(_origin) << LOGDEL
            << DTLOGVEC3D(_rotVector) << LOGDEL
            << DTLOGEVAL(_angle) << LOGDEL
            << DTLOGEVAL(_nPieces) );
  }
}