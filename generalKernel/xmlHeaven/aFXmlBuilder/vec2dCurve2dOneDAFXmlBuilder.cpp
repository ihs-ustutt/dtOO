#include "vec2dCurve2dOneDAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec2dCurve2dOneDAFXmlBuilder::vec2dCurve2dOneDAFXmlBuilder() {
  }

  vec2dCurve2dOneDAFXmlBuilder::~vec2dCurve2dOneDAFXmlBuilder() {

  }

  void vec2dCurve2dOneDAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    //
    //check input
    //
    bool hasOrder = dtXmlParserBase::hasAttribute("order", toBuildP);
    bool hasPoints = dtXmlParserBase::hasChild("Point_2", toBuildP);

    if ( hasPoints && hasOrder ) {
			std::vector< dtPoint2 > pointsArray;
      ::QDomElement elementP = dtXmlParserBase::getChild("Point_2", toBuildP);
      //
      //set input
      //
      int order = dtXmlParserBase::getAttributeInt("order", toBuildP);
      while ( !elementP.isNull() ) {
				std::vector< dtPoint2 > workingPoint;
        dtXmlParserBase::dtXmlParserBase::createBasic( &elementP, bC, cValP, depSFunP, &workingPoint );
        for (int ii=0;ii<workingPoint.size();ii++) {
          // put in point builder
					pointsArray.push_back(workingPoint[ii]);
        }
      // next sibling
      elementP = ::QDomElement( dtXmlParserBase::getNextSibling("Point_2", elementP) );      
      }
			ptrHandling<dtCurve2d> dtC2d( 
				bSplineCurve2d_pointConstructOCC(pointsArray, order).result() 
			);

      //
      // create scaCurve2dOneD
      //
      sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );			
    }
    else {
      dt__throw(
				buildPart(),
        << dt__eval(hasOrder) << std::endl
        << dt__eval(hasPoints) 
			);
    }
  }
}
