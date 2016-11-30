#include "discreteAddNormal.h"
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dOneD.h>
#include <analyticFunctionHeaven/aFBuilder/vec3dTwoD_normalOffset.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
  discreteAddNormal::discreteAddNormal() : dtTransformer() {
  }

  discreteAddNormal::discreteAddNormal(
    discreteAddNormal const & orig
  ) : dtTransformer(orig) {
    _tt.reset( orig._tt->clone() );
    _nf = orig._nf;
    _nU = orig._nU;
    _nV = orig._nV;
    _order = orig._order;
  }	
  
  discreteAddNormal::~discreteAddNormal() {
  }

  dtTransformer * discreteAddNormal::clone( void ) const {
	  return new discreteAddNormal(*this);
	}
	
  dtTransformer * discreteAddNormal::create( void ) const {
		return new discreteAddNormal();
	}
	
  aFPtrVec discreteAddNormal::apply( 
	  aFPtrVec const * const aFP 
	) const {
		aFPtrVec retV;
		
    dt__forAllIndex(*aFP, ii) {
			analyticFunction * aF = aFP->at(ii);

			vec3dTwoD const * const vec3d2d = vec3dTwoD::ConstDownCast(aF);
      if (vec3d2d) {
        vec3dTwoD_normalOffset anOffset( 
          vec3d2d, _tt.get(), _nf, _nU, _nV, _order
        );       
        retV.push_back( anOffset.result() );
        
        //
        // append wire if debug option set
        //
        if ( optionHandling::optionTrue("debug") ) {
          dt__forAllRefAuto(anOffset.resultWire(), aCurve) {
            retV.push_back( aCurve.clone() );
          }
        }
			}
			else dt__throwUnexpected(apply());
		}
		
		return retV;		
  }

  void discreteAddNormal::init( 
		::QDomElement const * tE, 
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG 
	) {
    dtTransformer::init(tE, bC, cV, aF, aG);
				
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("function_label", *tE)
      ||
      !dtXmlParserBase::hasAttribute("number_points_one", *tE)
      || 
      !dtXmlParserBase::hasAttribute("number_points_two", *tE)
      ||
      !dtXmlParserBase::hasAttribute("order", *tE)
      ||
      !dtXmlParserBase::hasAttribute("nf", *tE),
      init()
    );
    //
    // get functions
    //  
		dt__ptrAss(
      vec3dTwoD const * const tt,
      vec3dTwoD::ConstDownCast(
        aF->get( dtXmlParserBase::getAttributeStr("function_label", *tE) )
      )
    );
    _tt.reset(tt->clone());
       
    //
    // get number of points
    //
		_nU
    =
    dtXmlParserBase::getAttributeIntMuParse("number_points_one", *tE, cV, aF);
  	_nV 
    = 
	  dtXmlParserBase::getAttributeIntMuParse("number_points_two", *tE, cV, aF);

    //
    // get order
    //
		_order = dtXmlParserBase::getAttributeIntMuParse("order", *tE, cV, aF);
    
		//
		// get vector
		//
    _nf
    =
    dtXmlParserBase::getDtVector3(
      dtXmlParserBase::getAttributeStr("nf", *tE), bC 
    );    
  }

  bool discreteAddNormal::isNecessary( void ) const {
    return true;
  }
}

