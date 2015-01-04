#include "analyticAddNormal.h"
#include <baseContainerHeaven/baseContainer.h>
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/vec3dTwoD.h>
#include <functionHeaven/vec3dThickedTwoD.h>
#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  analyticAddNormal::analyticAddNormal() : dtTransformer() {
  }

  analyticAddNormal::analyticAddNormal(analyticAddNormal const & orig) : dtTransformer(orig) {
    _tt.reset( orig._tt->clone() );
    _nf = orig._nf;
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

//			vec2dOneD const * const vec2d1d = vec2dOneD::ConstDownCast(aF);			
			vec3dTwoD const * const vec3d2d = vec3dTwoD::ConstDownCast(aF);
			
//			if (vec2d1d) {			
//				analyticFunctionTransformed<vec2dCurve2dOneD> * aFT
//				= new analyticFunctionTransformed<vec2dCurve2dOneD>(*vec2dC1d);
//				aFT->setTransformer(this);
//				retV.push_back( aFT );
//				retV.back()->setLabel(aF->getLabel());
//			}			
      if (vec3d2d) {
				retV.push_back( 
					new vec3dThickedTwoD(vec3d2d, _tt.get(), _nf) 
				);
				retV.back()->setLabel(aF->getLabel());				
			}
			else {
				dt__THROW(
					apply(),
//					<< DTLOGEVAL(vec2d1d) << LOGDEL
					<< DTLOGEVAL(vec3d2d) << LOGDEL
					<< "Unknown type."
				);
			}
		}
		
		return retV;		
  }

  void analyticAddNormal::init( 
		QDomElement const * transformerElementP, 
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cValP,
		vectorHandling< analyticFunction * > const * const sFunP
	) {
    dtTransformer::init(transformerElementP, bC, cValP, sFunP);
				
    //
    // get functions
    //     
		if ( hasAttribute("function_label", *transformerElementP) ) {
			handleAnalyticFunction(
				"function_label", 
				sFunP->get( getAttributeStr("function_label", *transformerElementP) ) 
			);
		}
		//
		// set vector
		//
		_nf = dtVector3(0,0,0);
		if ( hasAttribute("scale_vector", *transformerElementP) ) {
			handleDtVector3(
				"scale_vector",
				getDtVector3(
					getAttributeStr("scale_vector", *transformerElementP), bC 
				)
			);
		}
  }

  bool analyticAddNormal::isNecessary( void ) const {
    return true;
  }
  
  void analyticAddNormal::handleAnalyticFunction(std::string const name, analyticFunction const * value) {
		vec3dTwoD const * const tt = vec3dTwoD::ConstDownCast(value);

    if (name == "function_label") {
      _tt.reset(tt->clone());
			return;
    }
    dtTransformer::handleAnalyticFunction(name, value);
  }  

  void analyticAddNormal::handleDtVector3(std::string const name, dtVector3 const value) {
    if (name == "scale_vector" ) {
      _nf = value;
      return;
    }
    dtTransformer::handleDtVector3(name, value);
  }		
}

