#include "vec3dMuParserThreeDFunctionDecorator.h"
#include <analyticFunctionHeaven/vec3dMuParserThreeD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  vec3dMuParserThreeDFunctionDecorator::vec3dMuParserThreeDFunctionDecorator() {
  }

  vec3dMuParserThreeDFunctionDecorator::~vec3dMuParserThreeDFunctionDecorator() {
  }

  void vec3dMuParserThreeDFunctionDecorator::buildPart(
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
    bool hasXThree = hasChild("x_three", toBuildP);				
    
    if (hasExpression && hasXOne && hasXTwo && hasXThree) {		
			QDomElement expEl = getChild("expression", toBuildP);
			QDomElement xOneEl = getChild("x_one", toBuildP);
			QDomElement xTwoEl = getChild("x_two", toBuildP);
			QDomElement xThreeEl = getChild("x_three", toBuildP);
			
			std::string expStr 
			= 
			replaceUsedFunctions(getAttributeStr("value", expEl) ,cValP, sFunP);
			
			vec3dMuParserThreeD * v3d 
			= 
			new vec3dMuParserThreeD(
			  expStr, 
			  getAttributeStr("name", xOneEl), 
				getAttributeStr("name", xTwoEl), 
				getAttributeStr("name", xThreeEl)
			); 
			
			v3d->setMin(0, getAttributeFloatMuParse("min", xOneEl, cValP, sFunP));
			v3d->setMin(1, getAttributeFloatMuParse("min", xTwoEl, cValP, sFunP));
			v3d->setMin(2, getAttributeFloatMuParse("min", xThreeEl, cValP, sFunP));
			v3d->setMax(0, getAttributeFloatMuParse("max", xOneEl, cValP, sFunP));
			v3d->setMax(1, getAttributeFloatMuParse("max", xTwoEl, cValP, sFunP));
			v3d->setMax(2, getAttributeFloatMuParse("max", xThreeEl, cValP, sFunP));
			sFunP->push_back(v3d);
    }
    else {
      dt__THROW(
				buildPart(), 
        << DTLOGEVAL(hasExpression) << LOGDEL
        << DTLOGEVAL(hasXOne) << LOGDEL
        << DTLOGEVAL(hasXTwo) << LOGDEL
				<< DTLOGEVAL(hasXThree) 
			);
    }
  }
}