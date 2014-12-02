#include "vec2dCurve2dOneDFunctionDecorator.h"
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <functionHeaven/analyticFunction.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <functionHeaven/vec2dCurve2dOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec2dCurve2dOneDFunctionDecorator::vec2dCurve2dOneDFunctionDecorator() {
  }

  vec2dCurve2dOneDFunctionDecorator::~vec2dCurve2dOneDFunctionDecorator() {

  }

  void vec2dCurve2dOneDFunctionDecorator::buildPart(
		QDomElement const & toBuildP, 
		baseContainer * const bC,
		vectorHandling< constValue * > const * const cValP, 
		vectorHandling< analyticFunction * > const * const depSFunP,
		vectorHandling< analyticFunction * > * sFunP 
	) const {
    //
    //check input
    //
    bool hasOrder = hasAttribute("order", toBuildP);
    bool hasPoints = hasChild("Point_2", toBuildP);
    bool hasScaFunction = hasChild("function", toBuildP);
    

    if ( hasPoints && hasOrder ) {
			std::vector< dtPoint2 > pointsArray;
      QDomElement elementP = getChild("Point_2", toBuildP);
      //
      //set input
      //
      int order = getAttributeInt("order", toBuildP);
      while ( !elementP.isNull() ) {
				std::vector< dtPoint2* > workingPoint;
        this->createBasic( &elementP, cValP, depSFunP, &workingPoint );
        for (int ii=0;ii<workingPoint.size();ii++) {
          // put in point builder
					pointsArray.push_back( *(workingPoint[ii]) );
					delete workingPoint[ii];
        }
      // next sibling
      elementP = QDomElement( getNextSibling("Point_2", elementP) );      
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
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasPoints) << LOGDEL
              << DTLOGEVAL(hasScaFunction) );
    }
  }
}
