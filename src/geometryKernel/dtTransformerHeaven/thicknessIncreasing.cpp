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

#include "thicknessIncreasing.h"

#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/scaOneD.h>
#include <analyticFunctionHeaven/vec2dOneD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include "dtTransformerFactory.h"

#include <logMe/dtMacros.h>
#include <logMe/logMe.h>

namespace dtOO {
  bool thicknessIncreasing::_registrated 
  =
  dtTransformerFactory::registrate(
    dt__tmpPtr(thicknessIncreasing, new thicknessIncreasing())
  );
  
  thicknessIncreasing::thicknessIncreasing() : dtTransformer() {
    _nPointsOne = 0;
    _isInv = false;
    _splineOrder = 3;    
  }

  thicknessIncreasing::~thicknessIncreasing() {
  }
	
  thicknessIncreasing::thicknessIncreasing(
    const thicknessIncreasing& orig
  ) : dtTransformer(orig) {
		_thicknessDistributionP.reset( 
		scaOneD::SecureCast(orig._thicknessDistributionP->clone())
		);
		_paraOnePercentFunP.reset( 
		  scaOneD::SecureCast(orig._paraOnePercentFunP->clone())
		);
		_nPointsOne = orig._nPointsOne;
		_isInv = orig._isInv;
		_splineOrder= orig._splineOrder;
	}

  dtTransformer * thicknessIncreasing::clone( void ) const {
	  return new thicknessIncreasing(*this);	
	}
	
  dtTransformer * thicknessIncreasing::create( void ) const {
		return new thicknessIncreasing();
	}
		
  lvH_analyticFunction thicknessIncreasing::apply( 
    lvH_analyticFunction const * const sFunP 
  ) const {
    lvH_analyticFunction transSFun;
    for (int ii=0;ii<sFunP->size();ii++) {
			std::vector< dtPoint2 > p2;
      //
      // cast pointer and check if it is a analyticFunction
      //
      dt__ptrAss(
        vec2dOneD const * theF, 
        vec2dOneD::ConstDownCast( sFunP->at(ii) ) 
      );

			std::vector<dtReal> itVal;       
			std::vector< std::string > header;
			header.push_back("Y_x");
			header.push_back("Y_y");
			header.push_back("N_x");
			header.push_back("N_y");
			header.push_back("tt");
			header.push_back("L_i/L");

      //
      // thickness increasing
      //            
			dtReal cLength = theF->length();
			dt__info(apply(), << dt__eval(cLength) );
      dtReal xMin = _paraOnePercentFunP->xMin(0);
      dtReal xMax = _paraOnePercentFunP->xMax(0);
      //
      // first point
      //
      dtReal paraOne = xMin;//0.;
      dtPoint2 YY = theF->YdtPoint2Percent( paraOne );
      dtVector2 NN = theF->unitNdtVector2Percent( paraOne );
      dtReal tt = _thicknessDistributionP->YFloat( 0. );
      if (_isInv) {
        tt = -tt;
      }
			p2.push_back( YY + tt * NN );
			itVal.push_back(YY.x()); itVal.push_back(YY.y());
			itVal.push_back(NN.x()); itVal.push_back(NN.y());
			itVal.push_back(tt);
			itVal.push_back(0.);
      //
      // inner points
      //
      for (int jj=1;jj<(_nPointsOne-1);jj++) {
        dt__toFloat(dtReal jjF, jj);
        dt__toFloat(dtReal nPointsOneF, _nPointsOne);
        paraOne 
        = 
        _paraOnePercentFunP->YFloat( jjF * ( (xMax-xMin) / (nPointsOneF-1.)) );
        YY = theF->YdtPoint2Percent( paraOne );
        NN = theF->unitNdtVector2Percent( paraOne );
				dtReal curLength = theF->length( theF->x_percent(paraOne) );
        tt = _thicknessDistributionP->YFloat( curLength / cLength );
        if (_isInv) {
          tt = -tt;
        }
			  p2.push_back( YY + tt * NN );
				itVal.push_back(YY.x()); itVal.push_back(YY.y());
				itVal.push_back(NN.x()); itVal.push_back(NN.y());
				itVal.push_back(tt);
				itVal.push_back(curLength / cLength);				
      }
      //
      // last point
      //
      paraOne = xMax;
      YY = theF->YdtPoint2Percent( paraOne );
      NN = theF->unitNdtVector2Percent( paraOne );
      tt = _thicknessDistributionP->YFloat( 1. );
      if (_isInv) {
        tt = -tt;
      }
			p2.push_back( YY + tt * NN );
			itVal.push_back(YY.x()); itVal.push_back(YY.y());
			itVal.push_back(NN.x()); itVal.push_back(NN.y());
			itVal.push_back(tt);
			itVal.push_back(1.);				
    
			dt__debug( apply(), << logMe::vecToTable(header, itVal) );
      //
      // create new function
      //
      ptrHandling<dtCurve2d> dtC2d( 
			  bSplineCurve2d_pointConstructOCC(p2, _splineOrder).result()
			);

      //
      // create scaCurve2dOneD
      //
      transSFun.push_back( new vec2dCurve2dOneD( dtC2d.get() ) );	
      transSFun.back()->setLabel( sFunP->at(ii)->getLabel() );
    }
    return transSFun;  
  }

  void thicknessIncreasing::init( 
	  ::QDomElement const * tE, 
		baseContainer * const bC,
    lvH_constValue const * const cV,
    lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG 					
	) {
		dtTransformer::init(tE, bC, cV, aF, aG);
		
    handleBool("inverted", dtXmlParserBase::getAttributeBool("inverted", *tE));
    handleInt("order", dtXmlParserBase::getAttributeInt("order", *tE));     
    
    //
    // get functions
    //     
    handleAnalyticFunction(
      "function_label", 
      aF->get( dtXmlParserBase::getAttributeStr("function_label", *tE) )->clone() 
    );
    handleAnalyticFunction(
      "parameter_one_percent_function", 
      aF->get( 
        dtXmlParserBase::getAttributeStr("parameter_one_percent_function", *tE) 
      )->clone()
    );
    
    handleInt( 
      "number_points_one", 
      dtXmlParserBase::getAttributeIntMuParse("number_points_one", *tE, cV, aF)
    );
  }
		
  bool thicknessIncreasing::isNecessary( void ) const {
    return true;
  }

  void thicknessIncreasing::handleInt(std::string const name, dtInt const value) {
    if (name == "order" ) {
      _splineOrder = value;
      return;
    }
    else if (name == "number_points_one") {
      _nPointsOne = value;
      return;
    }
    dtTransformer::handleInt(name, value);
  }
  
  void thicknessIncreasing::handleAnalyticFunction(
    std::string const name, analyticFunction const * value
  ) {
    if (name == "function_label") {
      dt__ptrAss( scaOneD const * s1d, scaOneD::ConstDownCast(value) );
			_thicknessDistributionP.reset(s1d);
      return;
    }
    else if (name == "parameter_one_percent_function") {
      dt__ptrAss( scaOneD const * s1d, scaOneD::ConstDownCast(value) );
			_paraOnePercentFunP.reset(s1d);			
      return;
    }
    dtTransformer::handleAnalyticFunction(name, value);
  }
  
  void thicknessIncreasing::handleBool(
    std::string const name, bool const value
  ) {
    if (name == "inverted") {
      _isInv = value;
      return;
    }    
    dtTransformer::handleBool(name, value);
  }
}
