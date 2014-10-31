#include "biThicknessIncreasing.h"
#include "thicknessIncreasing.h"
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/scaOneD.h>
#include <functionHeaven/vec2dOneD.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <interfaceHeaven/ptrHandling.h>
#include <algorithm>
#include <progHelper.h>
#include <logMe/logMe.h>

namespace dtOO {
  biThicknessIncreasing::biThicknessIncreasing() {
    _tD[0] = NULL;
		_tD[1] = NULL;
    _para = NULL;
    _nPointsOne = 0;
    _splineOrder = 0;    
  }

  biThicknessIncreasing::~biThicknessIncreasing() {
  }

  vectorHandling< analyticFunction * > biThicknessIncreasing::apply( vectorHandling< analyticFunction * > const * const sFunP ) const {
    vectorHandling< analyticFunction * > transSFun;
    for (int ii=0;ii<sFunP->size();ii++) {
			std::vector< dtPoint2 > p2;
			std::vector< dtPoint2 > p2Inv;
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
			header.push_back("ttInv");
			header.push_back("L_i/L");
			header.push_back("p2_x");
			header.push_back("p2_y");
			header.push_back("p2Inv_x");
			header.push_back("p2Inv_y");

      //
      // thickness increasing
      //            
			float cLength = theF->length();
			DTINFOWF(apply(), << DTLOGEVAL(cLength) );
      float xMin = _para->xMin(0);
      float xMax = _para->xMax(0);
      //
      // points
      //
      for (int jj=0;jj<_nPointsOne;jj++) {
        dt__TOFLOAT(float jjF, jj);
        dt__TOFLOAT(float nPointsOneF, _nPointsOne);
        float paraOne = _para->YFloat( jjF * ( (xMax-xMin) / (nPointsOneF-1.)) ) ;
        dtPoint2 YY = theF->YdtPoint2Percent( paraOne );
        dtVector2 NN = theF->unitNdtVector2Percent( paraOne );
				float curLength = theF->length( theF->x_percent(paraOne) );
        float tt = _tD[0]->YFloat( curLength / cLength );
				float ttInv = _tD[1]->YFloat( curLength / cLength );
			  p2.push_back( YY + tt * NN );
				p2Inv.push_back( YY - ttInv * NN );
				itVal.push_back(YY.x()); itVal.push_back(YY.y());
				itVal.push_back(NN.x()); itVal.push_back(NN.y());
				itVal.push_back(tt); itVal.push_back(ttInv);
				itVal.push_back(curLength / cLength);		
				itVal.push_back(p2[jj].x()); itVal.push_back(p2[jj].y());
				itVal.push_back(p2Inv[jj].x()); itVal.push_back(p2Inv[jj].y());
      }
			DTDEBUGWF( apply(), << floatVecToTable(header, itVal) );
						
      //
      // create new function
      //
			std::reverse( p2Inv.begin(), p2Inv.end() );
			p2.erase( p2.begin() );
			p2.erase( p2.end() );
			std::vector< dtPoint2 > p2All( p2.size()+p2Inv.size() );
			int counter = 0;
			dt__FORALL(p2, ii,
			  p2All[counter] = p2[ii];
			  counter++;
			);
			dt__FORALL(p2Inv, ii,
			  p2All[counter] = p2Inv[ii];
			  counter++;
			);			
			p2All.push_back( p2All.front() );
			
      ptrHandling<dtCurve2d> dtC2d( 
			  bSplineCurve2d_pointConstructOCC(p2All, _splineOrder).result()
			);

//			DTINFOWF( apply(), << DTLOGEVAL(dtC2d->closed()) );			
      //
      // create scaCurve2dOneD
      //
      transSFun.push_back( new vec2dCurve2dOneD( dtC2d.get() ) );	
      transSFun.back()->setLabel( sFunP->at(ii)->getLabel() );
    }
    return transSFun;  
  }

  void biThicknessIncreasing::init( QDomElement * transformerElementP, 
                                  vectorHandling< constValue * > const * const cValP,
                                  vectorHandling< analyticFunction * > const * const sFunP) {

    handleInt("order", getAttributeInt("order", *transformerElementP));     
    
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
    handleAnalyticFunction(
      "parameter_one_percent_function", 
      sFunP->get( getAttributeStr("parameter_one_percent_function", *transformerElementP) ) 
    );
    
    handleInt( 
      "number_points_one", 
      getAttributeIntMuParse("number_points_one", *transformerElementP, cValP, sFunP)
    );		
  }

  bool biThicknessIncreasing::isNecessary( void ) const {
    return true;
  }

  void biThicknessIncreasing::handleInt(std::string const name, int const value) {
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
  
  void biThicknessIncreasing::handleAnalyticFunction(std::string const name, analyticFunction const * value) {
    if (name == "function_label") {
      dt__PTRASS( _tD[0], scaOneD::ConstDownCast(value) );
      return;
    }
    if (name == "function_label_inverted") {
      dt__PTRASS( _tD[1], scaOneD::ConstDownCast(value) );
      return;
    }		
    else if (name == "parameter_one_percent_function") {
      dt__PTRASS( _para, scaOneD::ConstDownCast( value ) );
      return;
    }
    dtTransformer::handleAnalyticFunction(name, value);
  }  
}

