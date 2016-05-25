#include "muParserAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/scaMuParserOneD.h>
#include <analyticFunctionHeaven/scaMuParserTwoD.h>
#include <analyticFunctionHeaven/scaMuParserThreeD.h>
#include <analyticFunctionHeaven/vec2dMuParserTwoD.h>
#include <analyticFunctionHeaven/vec3dMuParserOneD.h>
#include <analyticFunctionHeaven/vec3dMuParserTwoD.h>
#include <analyticFunctionHeaven/vec3dMuParserThreeD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <interfaceHeaven/ptrHandling.h>
#include <logMe/logMe.h>

#include <QtXml/QDomElement>

namespace dtOO {
  muParserAFXmlBuilder::muParserAFXmlBuilder() {
  }

  muParserAFXmlBuilder::~muParserAFXmlBuilder() {
  }

  void muParserAFXmlBuilder::buildPart(
	  ::QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP
	) const {
							
    //
    // x-dimension
    //
    bool hasXOne = dtXmlParserBase::hasChild("x_one", toBuildP);
		bool hasXTwo = dtXmlParserBase::hasChild("x_two", toBuildP);
    bool hasXThree = dtXmlParserBase::hasChild("x_three", toBuildP);
		bool isOneD = (hasXOne && !hasXTwo && !hasXThree);
		bool isTwoD = (hasXOne && hasXTwo && !hasXThree);
		bool isThreeD = (hasXOne && hasXTwo && hasXThree);

		//
		// y-dimension
		//
    int yDim = 0;
		std::string expStr = "";
		if ( dtXmlParserBase::hasChild("expression", toBuildP) ) {
			::QDomElement expEl = dtXmlParserBase::getChild("expression", toBuildP);
			expStr 
			= 
			dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr("value", expEl), cValP, depSFunP
      );
			yDim = dtXmlParserBase::getAttributeInt("dimension", expEl);
		}
		bool isSca = (yDim==1);
		bool isVec2d = (yDim==2);
		bool isVec3d = (yDim==3);
		
		//
		// scaMuParserOneD
		//
    if (isSca && isOneD) {		
			::QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);

			scaMuParserOneD * theF 
			= 
			new scaMuParserOneD(
			  expStr, 
				dtXmlParserBase::getAttributeStr("name", xOneEl)
			); 
			theF->setMin(
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xOneEl, cValP, depSFunP
        )
      );
			theF->setMax(
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xOneEl, cValP, depSFunP
        )
      );

			sFunP->push_back(theF);
    }
		//
		// scaMuParserTwoD
		//		
    else if (isSca && isTwoD) {		
			::QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);
			::QDomElement xTwoEl = dtXmlParserBase::getChild("x_two", toBuildP);

			scaMuParserTwoD * theF 
			= 
			new scaMuParserTwoD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl), 
				dtXmlParserBase::getAttributeStr("name", xTwoEl)
			); 
			
			theF->setMin(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xOneEl, cValP, depSFunP
        )
      );
			theF->setMin(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xTwoEl, cValP, depSFunP
        )
      );
			theF->setMax(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xOneEl, cValP, depSFunP
        )
      );
			theF->setMax(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xTwoEl, cValP, depSFunP
        )
      );

			sFunP->push_back(theF);
    }		
		//
		// scaMuParserThreeD
		//		
    else if (isSca && isThreeD) {		
			::QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);
			::QDomElement xTwoEl = dtXmlParserBase::getChild("x_two", toBuildP);
      ::QDomElement xThreeEl = dtXmlParserBase::getChild("x_three", toBuildP);

			scaMuParserThreeD * theF 
			= 
			new scaMuParserThreeD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl), 
				dtXmlParserBase::getAttributeStr("name", xTwoEl),
        dtXmlParserBase::getAttributeStr("name", xThreeEl)
			); 
			
			theF->setMin(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xOneEl, cValP, depSFunP
        )
      );
			theF->setMin(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xTwoEl, cValP, depSFunP
        )
      );
			theF->setMin(
        2, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xThreeEl, cValP, depSFunP
        )
      );
      
			theF->setMax(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xOneEl, cValP, depSFunP
        )
      );
			theF->setMax(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xTwoEl, cValP, depSFunP
        )
      );
			theF->setMax(
        2, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xThreeEl, cValP, depSFunP
        )
      );
			sFunP->push_back(theF);
    }		    
		//
		// vec2dMuParserTwoD
		//
    else if (isVec2d && isTwoD) {		
			::QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);
			::QDomElement xTwoEl = dtXmlParserBase::getChild("x_two", toBuildP);

			vec2dMuParserTwoD * theF 
			= 
			new vec2dMuParserTwoD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl), 
				dtXmlParserBase::getAttributeStr("name", xTwoEl)
			); 
			
			theF->setMin(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xOneEl, cValP, depSFunP
        )
      );
			theF->setMin(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xTwoEl, cValP, depSFunP
        )
      );
			theF->setMax(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xOneEl, cValP, depSFunP
        )
      );
			theF->setMax(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xTwoEl, cValP, depSFunP
        )
      );

			sFunP->push_back(theF);
    }				
		//
		// vec3dMuParserOneD
		//
    else if (isVec3d && isOneD) {		
			::QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);

			vec3dMuParserOneD * theF 
			= 
			new vec3dMuParserOneD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl)
			); 
			
			theF->setMin(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xOneEl, cValP, depSFunP
        )
      );
			theF->setMax(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xOneEl, cValP, depSFunP
        )
      );

			sFunP->push_back(theF);
    }		    
		//
		// vec3dMuParserTwoD
		//
    else if (isVec3d && isTwoD) {		
			::QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);
			::QDomElement xTwoEl = dtXmlParserBase::getChild("x_two", toBuildP);

			vec3dMuParserTwoD * theF 
			= 
			new vec3dMuParserTwoD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl), 
				dtXmlParserBase::getAttributeStr("name", xTwoEl)
			); 
			
			theF->setMin(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xOneEl, cValP, depSFunP
        )
      );
			theF->setMin(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xTwoEl, cValP, depSFunP
        )
      );
			theF->setMax(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xOneEl, cValP, depSFunP
        )
      );
			theF->setMax(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xTwoEl, cValP, depSFunP
        )
      );

			sFunP->push_back(theF);
    }		
		//
		// vec3dMuParserThreeD
		//
    else if (isVec3d && isThreeD) {		
			::QDomElement xOneEl = dtXmlParserBase::getChild("x_one", toBuildP);
			::QDomElement xTwoEl = dtXmlParserBase::getChild("x_two", toBuildP);
			::QDomElement xThreeEl = dtXmlParserBase::getChild("x_three", toBuildP);

			vec3dMuParserThreeD * theF 
			= 
			new vec3dMuParserThreeD(
			  expStr, 
			  dtXmlParserBase::getAttributeStr("name", xOneEl), 
				dtXmlParserBase::getAttributeStr("name", xTwoEl),
				dtXmlParserBase::getAttributeStr("name", xThreeEl)
			); 
			
			theF->setMin(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xOneEl, cValP, depSFunP
        )
      );
			theF->setMin(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xTwoEl, cValP, depSFunP
        )
      );
			theF->setMin(
        2, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "min", xThreeEl, cValP, depSFunP
        )
      );
			theF->setMax(
        0, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xOneEl, cValP, depSFunP
        )
      );
			theF->setMax(
        1, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xTwoEl, cValP, depSFunP
        )
      );
			theF->setMax(
        2, 
        dtXmlParserBase::getAttributeFloatMuParse(
          "max", xThreeEl, cValP, depSFunP
        )
      );

			sFunP->push_back(theF);
    }				
    else {
      dt__throw(
				buildPart(), 
        << dt__eval(hasXOne) << std::endl
        << dt__eval(hasXTwo) << std::endl
        << dt__eval(hasXThree) << std::endl
        << dt__eval(isOneD) << std::endl
        << dt__eval(isTwoD) << std::endl
        << dt__eval(isThreeD) << std::endl
        << dt__eval(yDim) << std::endl
        << dt__eval(isSca) << std::endl
        << dt__eval(isVec2d) << std::endl
        << dt__eval(isVec3d) 
			);
    }
  }
}