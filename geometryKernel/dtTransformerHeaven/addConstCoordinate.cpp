#include "addConstCoordinate.h"
#include <logMe/logMe.h>
#include <progHelper.h>

#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomCurve_convertGeomCurve2d.h>

namespace dtOO {
  addConstCoordinate::addConstCoordinate() : dtTransformer() {
  }

  addConstCoordinate::addConstCoordinate(const addConstCoordinate& orig) : dtTransformer(orig) {
		_cc = orig._cc;
		_vv = orig._vv;
  }

  addConstCoordinate::~addConstCoordinate() {
  }

  dtTransformer * addConstCoordinate::clone( void ) const {
	  return new addConstCoordinate(*this);
	}
	
  dtTransformer * addConstCoordinate::create( void ) const {
		return new addConstCoordinate();
	}
	
  vectorHandling< analyticFunction * > addConstCoordinate::apply( 
	  vectorHandling< analyticFunction * > const * const aFVecP 
	) const {
    vectorHandling< analyticFunction * > aFVecRet;
//    
    for (int ii=0;ii<aFVecP->size();ii++) {
      dt__PTRASS(vec2dCurve2dOneD const * v2d1d, vec2dCurve2dOneD::DownCast(aFVecP->at(ii)));

			dt__pH(dtCurve) dtC(
			  geomCurve_convertGeomCurve2d(v2d1d->ptrDtCurve2d(), _vv).result()
			);
			aFVecRet.push_back( new vec3dCurveOneD(dtC.get()) );
    }
    
    return aFVecRet;
  }

  bool addConstCoordinate::isNecessary( void ) const {
    return true;
  }
  
  void addConstCoordinate::init( 
	  QDomElement * transformerElementP, 
    vectorHandling< constValue * > const * const cValP,
    vectorHandling< analyticFunction * > const * const sFunP
  ) {
    dtTransformer::init(transformerElementP, cValP, sFunP);
		
		float cc 
		= 
		muParseString( 
			replaceUsedFunctions(
				getAttributeStr("coordinate_value", *transformerElementP),
				cValP, 
				sFunP
			) 
		);
		handleFloat("coordinate_value", cc);

		QDomElement vvEl = getChild("Vector_3", *transformerElementP);
		dtVector3 vv = getDtVector3(vvEl, cValP, sFunP);
		handleDtVector3("dtVector3", vv);
  }
	
	void addConstCoordinate::handleFloat(const std::string name, const float value) {
    if (name == "coordinate_value" ) {
      _cc = value;
      return;
    }
    dtTransformer::handleFloat(name, value);		
	}

	void addConstCoordinate::handleDtVector3(const std::string name, const dtVector3 value) {
    if (name == "dtVector3" ) {
      _vv = value;
      return;
    }
    dtTransformer::handleDtVector3(name, value);		
	}	
}
