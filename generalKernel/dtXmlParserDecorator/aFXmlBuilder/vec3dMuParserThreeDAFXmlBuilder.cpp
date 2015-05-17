#include "vec3dMuParserThreeDAFXmlBuilder.h"

#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <analyticFunctionHeaven/vec3dMuParserThreeD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  vec3dMuParserThreeDAFXmlBuilder::vec3dMuParserThreeDAFXmlBuilder() {
  }

  vec3dMuParserThreeDAFXmlBuilder::~vec3dMuParserThreeDAFXmlBuilder() {
  }

  void vec3dMuParserThreeDAFXmlBuilder::buildPart(
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
    bool hasXThree = dtXmlParserBase::hasChild("x_three", toBuildP);				
    
    if (hasExpression && hasXOne && hasXTwo && hasXThree) {		
			QDomElement expEl = dtXmlParserBase::getChild("expression", toBuildP);
			QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);
			QDomElement xTwoEl = dtXmlParserBase::getChild("x_two", toBuildP);
			QDomElement xThreeEl = dtXmlParserBase::getChild("x_three", toBuildP);
			
			std::string expStr 
			= 
			dtXmlParserBase::replaceUsedFunctions(dtXmlParserBase::getAttributeStr("value", expEl) ,cValP, sFunP);
			
			vec3dMuParserThreeD * v3d 
			= 
			new vec3dMuParserThreeD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl), 
				dtXmlParserBase::getAttributeStr("name", xTwoEl), 
				dtXmlParserBase::getAttributeStr("name", xThreeEl)
			); 
			
			v3d->setMin(0, dtXmlParserBase::getAttributeFloatMuParse("min", xOneEl, cValP, sFunP));
			v3d->setMin(1, dtXmlParserBase::getAttributeFloatMuParse("min", xTwoEl, cValP, sFunP));
			v3d->setMin(2, dtXmlParserBase::getAttributeFloatMuParse("min", xThreeEl, cValP, sFunP));
			v3d->setMax(0, dtXmlParserBase::getAttributeFloatMuParse("max", xOneEl, cValP, sFunP));
			v3d->setMax(1, dtXmlParserBase::getAttributeFloatMuParse("max", xTwoEl, cValP, sFunP));
			v3d->setMax(2, dtXmlParserBase::getAttributeFloatMuParse("max", xThreeEl, cValP, sFunP));
			sFunP->push_back(v3d);
    }
    else {
      dt__throw(
				buildPart(), 
        << dt__eval(hasExpression) << std::endl
        << dt__eval(hasXOne) << std::endl
        << dt__eval(hasXTwo) << std::endl
				<< dt__eval(hasXThree) 
			);
    }
  }
}