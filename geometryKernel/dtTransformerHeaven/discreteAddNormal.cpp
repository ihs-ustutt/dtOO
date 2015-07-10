#include "discreteAddNormal.h"
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_normalOffset.h>
#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  discreteAddNormal::discreteAddNormal() : dtTransformer() {
  }

  discreteAddNormal::discreteAddNormal(
    discreteAddNormal const & orig
  ) : dtTransformer(orig) {
    _tt.reset( orig._tt->clone() );
    _nf = orig._nf;
  }	
  
  discreteAddNormal::~discreteAddNormal() {
  }

  dtTransformer * discreteAddNormal::clone( void ) const {
	  return new discreteAddNormal(*this);
	}
	
  dtTransformer * discreteAddNormal::create( void ) const {
		return new discreteAddNormal();
	}
	
  vectorHandling< analyticFunction * > discreteAddNormal::apply( 
	  vectorHandling< analyticFunction * > const * const aFP 
	) const {
		vectorHandling< analyticFunction * > retV;
		
    dt__forAllIndex(*aFP, ii) {
			analyticFunction * aF = aFP->at(ii);

			vec3dTwoD const * const vec3d2d = vec3dTwoD::ConstDownCast(aF);
      if (vec3d2d) {
				retV.push_back( 
					vec3dTwoD_normalOffset(
            vec3d2d, _tt.get(), _nf, _nU, _nV, _order
          ).result()
				);
				retV.back()->setLabel(aF->getLabel());				
			}
			else dt__throwUnexpected(apply());
		}
		
		return retV;		
  }

  void discreteAddNormal::init( 
		::QDomElement const * tE, 
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
				
    //
    // get functions
    //     
		if ( dtXmlParserBase::hasAttribute("function_label", *tE) ) {
			handleAnalyticFunction(
				"function_label", 
				aF->get( dtXmlParserBase::getAttributeStr("function_label", *tE) ) 
			);
		}
		//
		// set vector
		//
		_nf = dtVector3(0,0,0);
		if ( dtXmlParserBase::hasAttribute("scale_vector", *tE) ) {
			handleDtVector3(
				"scale_vector",
				dtXmlParserBase::getDtVector3(
					dtXmlParserBase::getAttributeStr("scale_vector", *tE), bC 
				)
			);
		}
    //
    // get number of points
    //
		_nU = 0;
    _nV = 0;
		if ( dtXmlParserBase::hasAttribute("number_points_one", *tE) ) {
			handleInt(
				"number_points_one",
				dtXmlParserBase::getAttributeIntMuParse(
					"number_points_one", *tE, cV, aF 
				)
			);
		}    
		if ( dtXmlParserBase::hasAttribute("number_points_two", *tE) ) {
			handleInt(
				"number_points_two",
				dtXmlParserBase::getAttributeIntMuParse(
					"number_points_two", *tE, cV, aF 
				)
			);
		}    
    //
    // get order
    //    
		if ( dtXmlParserBase::hasAttribute("order", *tE) ) {
			handleInt(
				"order",
				dtXmlParserBase::getAttributeIntMuParse(
					"order", *tE, cV, aF 
				)
			);
		}        
  }

  bool discreteAddNormal::isNecessary( void ) const {
    return true;
  }
  
  void discreteAddNormal::handleAnalyticFunction(
    std::string const name, analyticFunction const * value
  ) {
		vec3dTwoD const * const tt = vec3dTwoD::ConstDownCast(value);

    if (name == "function_label") {
      _tt.reset(tt->clone());
			return;
    }
    dtTransformer::handleAnalyticFunction(name, value);
  }  

  void discreteAddNormal::handleDtVector3(
    std::string const name, dtVector3 const value
  ) {
    if (name == "scale_vector" ) {
      _nf = value;
      return;
    }
    dtTransformer::handleDtVector3(name, value);
  }		

  void discreteAddNormal::handleInt(std::string const name, int const value) {
    if (name == "order" ) {
      _order = value;
      return;
    }
    else if (name == "number_points_one") {
      _nU = value;
      return;
    }
    else if (name == "number_points_two") {
      _nV = value;
      return;
    }    
    dtTransformer::handleInt(name, value);
  }  
}

