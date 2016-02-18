#include "translate.h"
#include "progHelper.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/dtCurve.h>
#include <logMe/logMe.h>

namespace dtOO {
  translate::translate() : dtTransformer() {
		_v2 = dtVector2(0,0);
		_v3 = dtVector3(0,0,0);
  }

	translate::translate( translate const & orig ) : dtTransformer(orig) {
		_v2 = orig._v2;
		_v3 = orig._v3;
	}
	
  translate::~translate() {
  }

  dtTransformer * translate::clone( void ) const {
	  return new translate(*this);
	}
	
  dtTransformer * translate::create( void ) const {
		return new translate();
	}

	std::vector< dtPoint3 > translate::apply( 
    std::vector< dtPoint3 > const * const toTrans 
  ) const {
		std::vector< dtPoint3 > ret(toTrans->size());
    
		dt__forAllIndex(*toTrans, ii) {
		  ret[ii] = toTrans->at(ii) + _v3;
		}
		
		return ret;
	}
  
  vectorHandling< analyticFunction * > translate::apply( 
    vectorHandling< analyticFunction * > const * const sFunP 
  ) const { 
    vectorHandling< analyticFunction * > retSFun;

		
    for (int ii=0; ii<sFunP->size(); ii++) {
      //
      // clone and cast scaFunction
      //
			scaCurve2dOneD  const * const sC2d1d 
      = 
      scaCurve2dOneD::ConstDownCast( sFunP->at(ii) );
		  vec2dCurve2dOneD  const * const v2dC1d 
      = 
      vec2dCurve2dOneD::ConstDownCast( sFunP->at(ii) );
			vec3dCurveOneD  const * const v3dC1d 
      = 
      vec3dCurveOneD::ConstDownCast( sFunP->at(ii) );
			
			if ( sC2d1d ) {
				dtCurve2d * dtC2d = sC2d1d->ptrDtCurve2d()->clone();
				dtC2d->translate(_v2);
				retSFun.push_back( new scaCurve2dOneD(dtC2d) );
				retSFun.back()->setLabel(sC2d1d->getLabel());
			}
			else if ( v2dC1d ) {
				dtCurve2d * dtC2d = v2dC1d->ptrDtCurve2d()->clone();
				dtC2d->translate(_v2);
				retSFun.push_back( new vec2dCurve2dOneD(dtC2d) );
				retSFun.back()->setLabel(v2dC1d->getLabel());
			}			
			else if ( v3dC1d ) {
				dtCurve * dtC = v3dC1d->ptrDtCurve()->clone();
				dtC->translate(_v3);
				retSFun.push_back( new vec3dCurveOneD(dtC) );
				retSFun.back()->setLabel(v3dC1d->getLabel());
			}
			else {
        dt__throw(
					apply(),
          << dt__eval(sC2d1d) << std::endl
          << dt__eval(v2dC1d) << std::endl
          << dt__eval(v3dC1d) << std::endl								
          << "Unknown type."
				);
      }					
    }
    return retSFun;    
  }

  bool translate::isNecessary( void ) const {
    return true;
  }

  void translate::init( 
	  ::QDomElement const * tE, 
    baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
		
    if ( dtXmlParserBase::hasChild("Vector_2", *tE) ) {
			::QDomElement v2El = dtXmlParserBase::getChild("Vector_2", *tE);
			_v2 = dtXmlParserBase::getDtVector2(&v2El, bC, cV, aF, aG);
    }
    if ( dtXmlParserBase::hasChild("Vector_3", *tE) ) {
			::QDomElement v3El = dtXmlParserBase::getChild("Vector_3", *tE);
			_v3 = dtXmlParserBase::getDtVector3(&v3El, bC, cV, aF, aG);
    }
  }
}