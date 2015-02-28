#include "muParserFunctionDecorator.h"
#include <functionHeaven/scaMuParserOneD.h>
#include <functionHeaven/scaMuParserTwoD.h>
#include <functionHeaven/vec3dMuParserTwoD.h>
#include <functionHeaven/vec2dMuParserTwoD.h>
#include <functionHeaven/vec3dMuParserThreeD.h>
#include <functionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  muParserFunctionDecorator::muParserFunctionDecorator() {
  }

  muParserFunctionDecorator::~muParserFunctionDecorator() {
  }

  void muParserFunctionDecorator::buildPart(
	  QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
							
    //
    // x-dimension
    //
    bool hasXOne = hasChild("x_one", toBuildP);
		bool hasXTwo = hasChild("x_two", toBuildP);
    bool hasXThree = hasChild("x_three", toBuildP);
		bool isOneD = (hasXOne && !hasXTwo && !hasXThree);
		bool isTwoD = (hasXOne && hasXTwo && !hasXThree);
		bool isThreeD = (hasXOne && hasXTwo && hasXThree);

		//
		// y-dimension
		//
    int yDim = 0;
		std::string expStr = "";
		if ( hasChild("expression", toBuildP) ) {
			QDomElement expEl = getChild("expression", toBuildP);
			expStr 
			= 
			replaceUsedFunctions(getAttributeStr("value", expEl), cValP, depSFunP);
			yDim = getAttributeInt("dimension", expEl);
		}
		bool isSca = (yDim==1);
		bool isVec2d = (yDim==2);
		bool isVec3d = (yDim==3);
		
		//
		// scaMuParserOneD
		//
    if (isSca && isOneD) {		
			QDomElement xOneEl = getChild("x_one", toBuildP);

			scaMuParserOneD * theF 
			= 
			new scaMuParserOneD(
			  expStr, 
				getAttributeStr("name", xOneEl)
			); 
			theF->setMin(getAttributeFloatMuParse("min", xOneEl, cValP, depSFunP));
			theF->setMax(getAttributeFloatMuParse("max", xOneEl, cValP, depSFunP));

			sFunP->push_back(theF);
    }
		//
		// scaMuParserTwoD
		//		
    else if (isSca && isTwoD) {		
			QDomElement xOneEl = getChild("x_one", toBuildP);
			QDomElement xTwoEl = getChild("x_two", toBuildP);

			scaMuParserTwoD * theF 
			= 
			new scaMuParserTwoD(
			  expStr, 
			  getAttributeStr("name", xOneEl), 
				getAttributeStr("name", xTwoEl)
			); 
			
			theF->setMin(0, getAttributeFloatMuParse("min", xOneEl, cValP, depSFunP));
			theF->setMin(1, getAttributeFloatMuParse("min", xTwoEl, cValP, depSFunP));
			theF->setMax(0, getAttributeFloatMuParse("max", xOneEl, cValP, depSFunP));
			theF->setMax(1, getAttributeFloatMuParse("max", xTwoEl, cValP, depSFunP));

			sFunP->push_back(theF);
    }		
		//
		// vec2dMuParserTwoD
		//
    else if (isVec2d && isTwoD) {		
			QDomElement xOneEl = getChild("x_one", toBuildP);
			QDomElement xTwoEl = getChild("x_two", toBuildP);

			vec2dMuParserTwoD * theF 
			= 
			new vec2dMuParserTwoD(
			  expStr, 
			  getAttributeStr("name", xOneEl), 
				getAttributeStr("name", xTwoEl)
			); 
			
			theF->setMin(0, getAttributeFloatMuParse("min", xOneEl, cValP, depSFunP));
			theF->setMin(1, getAttributeFloatMuParse("min", xTwoEl, cValP, depSFunP));
			theF->setMax(0, getAttributeFloatMuParse("max", xOneEl, cValP, depSFunP));
			theF->setMax(1, getAttributeFloatMuParse("max", xTwoEl, cValP, depSFunP));

			sFunP->push_back(theF);
    }				
		//
		// vec3dMuParserTwoD
		//
    else if (isVec3d && isTwoD) {		
			QDomElement xOneEl = getChild("x_one", toBuildP);
			QDomElement xTwoEl = getChild("x_two", toBuildP);

			vec3dMuParserTwoD * theF 
			= 
			new vec3dMuParserTwoD(
			  expStr, 
			  getAttributeStr("name", xOneEl), 
				getAttributeStr("name", xTwoEl)
			); 
			
			theF->setMin(0, getAttributeFloatMuParse("min", xOneEl, cValP, depSFunP));
			theF->setMin(1, getAttributeFloatMuParse("min", xTwoEl, cValP, depSFunP));
			theF->setMax(0, getAttributeFloatMuParse("max", xOneEl, cValP, depSFunP));
			theF->setMax(1, getAttributeFloatMuParse("max", xTwoEl, cValP, depSFunP));

			sFunP->push_back(theF);
    }		
		//
		// vec3dMuParserThreeD
		//
    else if (isVec3d && isThreeD) {		
			QDomElement xOneEl = getChild("x_one", toBuildP);
			QDomElement xTwoEl = getChild("x_two", toBuildP);
			QDomElement xThreeEl = getChild("x_three", toBuildP);

			vec3dMuParserThreeD * theF 
			= 
			new vec3dMuParserThreeD(
			  expStr, 
			  getAttributeStr("name", xOneEl), 
				getAttributeStr("name", xTwoEl),
				getAttributeStr("name", xThreeEl)
			); 
			
			theF->setMin(0, getAttributeFloatMuParse("min", xOneEl, cValP, depSFunP));
			theF->setMin(1, getAttributeFloatMuParse("min", xTwoEl, cValP, depSFunP));
			theF->setMin(2, getAttributeFloatMuParse("min", xThreeEl, cValP, depSFunP));
			theF->setMax(0, getAttributeFloatMuParse("max", xOneEl, cValP, depSFunP));
			theF->setMax(1, getAttributeFloatMuParse("max", xTwoEl, cValP, depSFunP));
			theF->setMax(2, getAttributeFloatMuParse("max", xThreeEl, cValP, depSFunP));

			sFunP->push_back(theF);
    }				
    else {
      dt__THROW(
				buildPart(), 
        << DTLOGEVAL(hasXOne) << LOGDEL
        << DTLOGEVAL(hasXTwo) << LOGDEL
        << DTLOGEVAL(hasXThree) << LOGDEL
        << DTLOGEVAL(isOneD) << LOGDEL
        << DTLOGEVAL(isTwoD) << LOGDEL
        << DTLOGEVAL(isThreeD) << LOGDEL
        << DTLOGEVAL(yDim) << LOGDEL
        << DTLOGEVAL(isSca) << LOGDEL
        << DTLOGEVAL(isVec2d) << LOGDEL
        << DTLOGEVAL(isVec3d) 
			);
    }
  }
}