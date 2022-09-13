#include "addConstCoordinate.h"
#include "baseContainerHeaven/baseContainer.h"
#include <logMe/logMe.h>
#include <logMe/dtMacros.h>

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/geomCurve_convertGeomCurve2d.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool addConstCoordinate::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(addConstCoordinate, new addConstCoordinate())
  );
  
  addConstCoordinate::addConstCoordinate() : dtTransformer() {
  }

  addConstCoordinate::addConstCoordinate(
    const addConstCoordinate& orig
  ) : dtTransformer(orig) {
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
	
  lvH_analyticFunction addConstCoordinate::apply( 
	  lvH_analyticFunction const * const aFVecP 
	) const {
    lvH_analyticFunction aFVecRet;
    
    for (int ii=0;ii<aFVecP->size();ii++) {
      dt__ptrAss(
        vec2dCurve2dOneD const * v2d1d, 
        vec2dCurve2dOneD::DownCast(aFVecP->at(ii))
      );

			dt__pH(dtCurve) dtC(
			  geomCurve_convertGeomCurve2d(
          v2d1d->ptrDtCurve2d(), CGAL::ORIGIN + _vv*_cc, _vv
        ).result()
			);
			aFVecRet.push_back( new vec3dCurveOneD(dtC.get()) );
    }
    
    return aFVecRet;
  }

  bool addConstCoordinate::isNecessary( void ) const {
    return true;
  }
  
  void addConstCoordinate::init( 
		::QDomElement const * tE, 
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
  ) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
		dtReal cc 
		= 
		dtXmlParserBase::muParseString( 
			dtXmlParserBase::replaceDependencies(
				dtXmlParserBase::getAttributeStr("coordinate_value", *tE),
				cV, 
				aF
			) 
		);
		handleFloat("coordinate_value", cc);

		dtVector3 vv 
    = 
    dtXmlParserBase::getDtVector3(
      dtXmlParserBase::getChild("Vector_3", *tE), bC
    );
		handleDtVector3("dtVector3", vv);
  }
	
	void addConstCoordinate::handleFloat(
    const std::string name, const dtReal value
  ) {
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
