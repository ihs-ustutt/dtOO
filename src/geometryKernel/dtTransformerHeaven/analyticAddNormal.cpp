/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "analyticAddNormal.h"
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec3dTwoD.h>
#include <analyticFunctionHeaven/vec3dThickedTwoD.h>
#include <logMe/dtMacros.h>
#include <logMe/logMe.h>
#include "dtTransformerFactory.h"

namespace dtOO {
  bool analyticAddNormal::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(analyticAddNormal, new analyticAddNormal())
  );
  
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
	
  lvH_analyticFunction analyticAddNormal::apply( 
	  lvH_analyticFunction const * const aFP 
	) const {
		lvH_analyticFunction retV;
		
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
				dt__throw(
					apply(),
//					<< dt__eval(vec2d1d) << std::endl
					<< dt__eval(vec3d2d) << std::endl
					<< "Unknown type."
				);
			}
		}
		
		return retV;		
  }

  void analyticAddNormal::init( 
		::QDomElement const * tE, 
		baseContainer * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 
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
