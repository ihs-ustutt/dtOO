#include "thicknessIncreasing.h"
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/scaOneD.h>
#include <functionHeaven/vec2dOneD.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
//#include <interfaceHeaven/labelHandling.h>

#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  thicknessIncreasing::thicknessIncreasing() {
    _thicknessDistributionP = NULL;
    _paraOnePercentFunP = NULL;
    _nPointsOne = 0;
    _isInv = false;
    _splineOrder = 3;    
  }

  thicknessIncreasing::~thicknessIncreasing() {
  }

  vectorHandling< analyticFunction * > thicknessIncreasing::apply( vectorHandling< analyticFunction * > const * const sFunP ) const {
    vectorHandling< analyticFunction * > transSFun;
    for (int ii=0;ii<sFunP->size();ii++) {
			std::vector< dtPoint2 > p2;
      //
      // cast pointer and check if it is a analyticFunction
      //
      dt__PTRASS(vec2dOneD const * theF, vec2dOneD::ConstDownCast( sFunP->at(ii) ) );

			std::vector<float> itVal;       
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
			float cLength = theF->length();
			DTINFOWF(apply(), << DTLOGEVAL(cLength) );
      float xMin = _paraOnePercentFunP->xMin(0);
      float xMax = _paraOnePercentFunP->xMax(0);
      //
      // first point
      //
      float paraOne = xMin;//0.;
      dtPoint2 YY = theF->YdtPoint2Percent( paraOne );
      dtVector2 NN = theF->unitNdtVector2Percent( paraOne );
      float tt = _thicknessDistributionP->YFloat( 0. );
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
        dt__TOFLOAT(float jjF, jj);
        dt__TOFLOAT(float nPointsOneF, _nPointsOne);
        paraOne = _paraOnePercentFunP->YFloat( jjF * ( (xMax-xMin) / (nPointsOneF-1.)) ) ;
        YY = theF->YdtPoint2Percent( paraOne );
        NN = theF->unitNdtVector2Percent( paraOne );
				float curLength = theF->length( theF->x_percent(paraOne) );
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
    
			DTDEBUGWF( apply(), << logMe::floatVecToTable(header, itVal) );
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

  void thicknessIncreasing::init( QDomElement * transformerElementP, 
                                  vectorHandling< constValue * > const * const cValP,
                                  vectorHandling< analyticFunction * > const * const sFunP) {

    handleBool("inverted", getAttributeBool("inverted", *transformerElementP));
    handleInt("order", getAttributeInt("order", *transformerElementP));     
    
    //
    // get functions
    //     
    handleAnalyticFunction(
      "function_label", 
      sFunP->get( getAttributeStr("function_label", *transformerElementP) ) 
    );
    handleAnalyticFunction(
      "parameter_one_percent_function", 
      sFunP->get( getAttributeStr("parameter_one_percent_function", *transformerElementP) ) 
    );
    
    handleInt( 
      "number_points_one", 
      getAttributeIntMuParse("number_points_one", *transformerElementP, cValP, sFunP)
    );
  }

  bool thicknessIncreasing::isNecessary( void ) const {
    return true;
  }

  void thicknessIncreasing::handleInt(std::string const name, int const value) {
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
  
  void thicknessIncreasing::handleAnalyticFunction(std::string const name, analyticFunction const * value) {
    if (name == "function_label") {
      dt__PTRASS( _thicknessDistributionP, scaOneD::ConstDownCast(value) );
      return;
    }
    else if (name == "parameter_one_percent_function") {
      dt__PTRASS( _paraOnePercentFunP, scaOneD::ConstDownCast( value ) );
      return;
    }
    dtTransformer::handleAnalyticFunction(name, value);
  }
  
  void thicknessIncreasing::handleBool(std::string const name, bool const value) {
    if (name == "inverted") {
      _isInv = value;
      return;
    }    
    dtTransformer::handleBool(name, value);
  }
}

