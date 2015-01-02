#include "analyticAddNormal.h"
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/scaOneD.h>
#include <functionHeaven/scaTwoD.h>
#include <functionHeaven/vec2dOneD.h>
#include <functionHeaven/vec3dTwoD.h>
#include <functionHeaven/vec3dThickedTwoD.h>
#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  analyticAddNormal::analyticAddNormal() : dtTransformer() {
  }

  analyticAddNormal::analyticAddNormal(analyticAddNormal const & orig) : dtTransformer(orig) {
		_reverse = orig._reverse;
		_s1d_tD0.reset( orig._s1d_tD0->clone() );
		_s1d_tD1.reset( orig._s1d_tD1->clone() );
		_s2d_tD0.reset( orig._s2d_tD0->clone() );
		_s2d_tD1.reset( orig._s2d_tD1->clone() );
  }	
  analyticAddNormal::~analyticAddNormal() {
  }

  dtTransformer * analyticAddNormal::clone( void ) const {
	  return new analyticAddNormal(*this);
	}
	
  dtTransformer * analyticAddNormal::create( void ) const {
		return new analyticAddNormal();
	}
	
  vectorHandling< analyticFunction * > analyticAddNormal::apply( 
	  vectorHandling< analyticFunction * > const * const aFP 
	) const {
		vectorHandling< analyticFunction * > retV;
		
    for (int ii=0; ii<aFP->size(); ii++) {
			analyticFunction * aF = aFP->at(ii);

			vec2dOneD const * const vec2d1d = vec2dOneD::ConstDownCast(aF);			
			vec3dTwoD const * const vec3d2d = vec3dTwoD::ConstDownCast(aF);
			
//			if (vec2d1d) {			
//				analyticFunctionTransformed<vec2dCurve2dOneD> * aFT
//				= new analyticFunctionTransformed<vec2dCurve2dOneD>(*vec2dC1d);
//				aFT->setTransformer(this);
//				retV.push_back( aFT );
//				retV.back()->setLabel(aF->getLabel());
//			}			
      if (vec3d2d) {			
				retV.push_back( new vec3dThickedTwoD(vec3d2d, _s2d_tD0.get()) );
				retV.back()->setLabel(aF->getLabel());				
			}
			else {
				dt__THROW(
					apply(),
					<< DTLOGEVAL(vec2d1d) << LOGDEL
					<< DTLOGEVAL(vec3d2d) << LOGDEL
					<< "Unknown type."
				);
			}
		}
		
		return retV;		
  }

  void analyticAddNormal::init( 
		QDomElement const * transformerElementP, 
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP
	) {
    dtTransformer::init(transformerElementP, cValP, sFunP);
		
    handleBool("reverse", getAttributeBool("reverse", *transformerElementP));		
				
    //
    // get functions
    //     
    handleAnalyticFunction(
      "function_label", 
      sFunP->get( getAttributeStr("function_label", *transformerElementP) ) 
    );
    handleAnalyticFunction(
      "function_label_inverted", 
      sFunP->get( getAttributeStr("function_label_inverted", *transformerElementP) ) 
    );		
  }

  bool analyticAddNormal::isNecessary( void ) const {
    return true;
  }

  void analyticAddNormal::handleBool(std::string const name, bool const value) {
    if (name == "reverse" ) {
      _reverse = value;
      return;
    }
    dtTransformer::handleBool(name, value);
  }	
  
  void analyticAddNormal::handleAnalyticFunction(std::string const name, analyticFunction const * value) {
  	scaOneD const * const s1d = scaOneD::ConstDownCast(value);
		scaTwoD const * const s2d = scaTwoD::ConstDownCast(value);

    if (name == "function_label") {
			if (s1d) {
				_s1d_tD0.reset( s1d->clone() );
				return;
			}
			else if (s2d) {
				_s2d_tD0.reset( s2d->clone() );
				return;
			}			
    }
    else if (name == "function_label_inverted") {
			if (s1d) {
				_s1d_tD1.reset( s1d->clone() );
				return;
			}
			else if (s2d) {
				_s2d_tD1.reset( s2d->clone() );
				return;
			}			
    }
    dtTransformer::handleAnalyticFunction(name, value);
  }  
}

