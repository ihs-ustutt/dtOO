#include "scaMuParserTwoDAFXmlBuilder.h"

#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <analyticFunctionHeaven/scaMuParserTwoD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  scaMuParserTwoDAFXmlBuilder::scaMuParserTwoDAFXmlBuilder() {
  }

  scaMuParserTwoDAFXmlBuilder::~scaMuParserTwoDAFXmlBuilder() {
  }

  void scaMuParserTwoDAFXmlBuilder::buildPart(
	  QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
							
    //
    //check input
    //
    bool hasExpression = dtXmlParserBase::hasChild("expression", toBuildP);
    bool hasXOne = dtXmlParserBase::hasChild("x_one", toBuildP);
		bool hasXTwo = dtXmlParserBase::hasChild("x_two", toBuildP);
    
    if (hasExpression && hasXOne && hasXTwo) {		
			QDomElement expEl = dtXmlParserBase::getChild("expression", toBuildP);
			QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);
			QDomElement xTwoEl = dtXmlParserBase::getChild("x_two", toBuildP);
			
			std::string expStr 
			= 
			dtXmlParserBase::replaceUsedFunctions(dtXmlParserBase::getAttributeStr("value", expEl) ,cValP, sFunP);
			
			scaMuParserTwoD * s2d 
			= 
			new scaMuParserTwoD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl), 
				dtXmlParserBase::getAttributeStr("name", xTwoEl)
			); 
			
			s2d->setMin(0, dtXmlParserBase::getAttributeFloatMuParse("min", xOneEl, cValP, sFunP));
			s2d->setMin(1, dtXmlParserBase::getAttributeFloatMuParse("min", xTwoEl, cValP, sFunP));
			s2d->setMax(0, dtXmlParserBase::getAttributeFloatMuParse("max", xOneEl, cValP, sFunP));
			s2d->setMax(1, dtXmlParserBase::getAttributeFloatMuParse("max", xTwoEl, cValP, sFunP));
			sFunP->push_back(s2d);
    }
    else {
      dt__throw(
				buildPart(), 
        << dt__eval(hasExpression) << std::endl
        << dt__eval(hasXOne) << std::endl
        << dt__eval(hasXTwo) 
			);
    }
  }
}