#include "scaMuParserTwoDFunctionDecorator.h"
#include <analyticFunctionHeaven/scaMuParserTwoD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  scaMuParserTwoDFunctionDecorator::scaMuParserTwoDFunctionDecorator() {
  }

  scaMuParserTwoDFunctionDecorator::~scaMuParserTwoDFunctionDecorator() {
  }

  void scaMuParserTwoDFunctionDecorator::buildPart(
	  QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
							
    //
    //check input
    //
    bool hasExpression = hasChild("expression", toBuildP);
    bool hasXOne = hasChild("x_one", toBuildP);
		bool hasXTwo = hasChild("x_two", toBuildP);
    
    if (hasExpression && hasXOne && hasXTwo) {		
			QDomElement expEl = getChild("expression", toBuildP);
			QDomElement xOneEl = getChild("x_one", toBuildP);
			QDomElement xTwoEl = getChild("x_two", toBuildP);
			
			std::string expStr 
			= 
			replaceUsedFunctions(getAttributeStr("value", expEl) ,cValP, sFunP);
			
			scaMuParserTwoD * s2d 
			= 
			new scaMuParserTwoD(
			  expStr, 
			  getAttributeStr("name", xOneEl), 
				getAttributeStr("name", xTwoEl)
			); 
			
			s2d->setMin(0, getAttributeFloatMuParse("min", xOneEl, cValP, sFunP));
			s2d->setMin(1, getAttributeFloatMuParse("min", xTwoEl, cValP, sFunP));
			s2d->setMax(0, getAttributeFloatMuParse("max", xOneEl, cValP, sFunP));
			s2d->setMax(1, getAttributeFloatMuParse("max", xTwoEl, cValP, sFunP));
			sFunP->push_back(s2d);
    }
    else {
      dt__THROW(
				buildPart(), 
        << DTLOGEVAL(hasExpression) << LOGDEL
        << DTLOGEVAL(hasXOne) << LOGDEL
        << DTLOGEVAL(hasXTwo) 
			);
    }
  }
}