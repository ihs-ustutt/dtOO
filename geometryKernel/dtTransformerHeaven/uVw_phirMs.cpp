#include "uVw_phirMs.h"
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/vec3dSurfaceTwoD.h>
#include <functionHeaven/vec3dCurveOneD.h>
#include <functionHeaven/analyticFunctionTransformed.h>

namespace dtOO {
  uVw_phirMs::uVw_phirMs() : dtTransformer() {
  }

  uVw_phirMs::uVw_phirMs(
	  uVw_phirMs const & orig 
	) : dtTransformer(orig) {
		_rM2d.reset( orig._rM2d->clone() );
		_ss = orig._ss;
  }
	
  uVw_phirMs::~uVw_phirMs() {
		
  }

  dtTransformer * uVw_phirMs::clone( void ) const {
	  return new uVw_phirMs(*this);
	}
	
  dtTransformer * uVw_phirMs::create( void ) const {
		return new uVw_phirMs();
	}
	
  vectorHandling< analyticFunction * > uVw_phirMs::apply( 
	  vectorHandling< analyticFunction * > const * const aFP 
	) const {
		vectorHandling< analyticFunction * > retV;
		
    for (int ii=0; ii<aFP->size(); ii++) {
			analyticFunction * aF = aFP->at(ii);

			vec3dCurveOneD const * const vec3d1d = vec3dCurveOneD::ConstDownCast(aF);			
			vec3dSurfaceTwoD const * const vec3d2d = vec3dSurfaceTwoD::ConstDownCast(aF);
			
			if (vec3d1d) {			
				analyticFunctionTransformed<vec3dCurveOneD> * aFT
				= new analyticFunctionTransformed<vec3dCurveOneD>(*vec3d1d);
				aFT->setTransformer(this);
				retV.push_back( aFT );
				retV.back()->setLabel(aF->getLabel());
			}			
      else if (vec3d2d) {			
				analyticFunctionTransformed<vec3dSurfaceTwoD> * aFT
				= new analyticFunctionTransformed<vec3dSurfaceTwoD>(*vec3d2d);
				aFT->setTransformer(this);
				retV.push_back( aFT );
				retV.back()->setLabel(aF->getLabel());				
			}
			else {
				dt__THROW(
					apply(),
					<< DTLOGEVAL(vec3d1d) << LOGDEL
					<< DTLOGEVAL(vec3d2d) << LOGDEL
					<< "Unknown type."
				);
			}
		}
		
		return retV;
  }

	std::vector< dtPoint3 > 
	uVw_phirMs::apply( std::vector< dtPoint3 > const * const toTrans ) const {
		std::vector< dtPoint3 > retVec;
		dt__FORALL(*toTrans, ii,
      float xx = toTrans->at(ii).x() * _ss.x();
		  float yy = toTrans->at(ii).y() * _ss.y();
		  float zz = toTrans->at(ii).z() * _ss.z();
		
      float ww = _rM2d->w_s(zz);						
			float vv = _rM2d->v_mw(yy, ww);
			float uu = _rM2d->u_phirvw(xx, vv, ww);
			
			retVec.push_back( dtPoint3(uu, vv, ww) );
		);
		
		return retVec;
	}
	
  bool uVw_phirMs::isNecessary( void ) const {
    return true;
  }

  void uVw_phirMs::init( 
	  QDomElement const * transformerElementP, 
    baseContainer * const bC,  
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP 
	) {
    dtTransformer::init(transformerElementP, bC, cValP, sFunP, depAGeoP);
		
		if ( hasChild("Vector_3", *transformerElementP) ) {
			QDomElement v3El = getChild("Vector_3", *transformerElementP);
			handleDtVector3(
				"Vector_3", 
				createDtVector3(&v3El, bC, cValP, sFunP, depAGeoP)
			);
		}
		else {
      handleDtVector3("Vector_3", dtVector3(1.,1.,1.));			
		}
		handleAnalyticGeometry(
			"part_label", 
			depAGeoP->get(getAttributeStr("part_label", *transformerElementP))
		);
  }
  
  void uVw_phirMs::handleAnalyticGeometry(std::string const name, analyticGeometry const * value) {
    if (name == "part_label") {
      dt__PTRASS(rotatingMap2dTo3d const * m3d, rotatingMap2dTo3d::ConstDownCast(value));
			_rM2d.reset( m3d->clone() );
      return;
    }
    dtTransformer::handleAnalyticGeometry(name, value);
  }
	
  void uVw_phirMs::handleDtVector3(std::string const name, dtVector3 const value) {
    if (name == "Vector_3") {
      _ss = value;
      return;
    }
    dtTransformer::handleDtVector3(name, value);
  }	
}