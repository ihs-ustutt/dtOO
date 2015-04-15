#include "vec3dCurveOneDFunctionDecorator.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dCurveOneDFunctionDecorator::vec3dCurveOneDFunctionDecorator() {
  }

  vec3dCurveOneDFunctionDecorator::~vec3dCurveOneDFunctionDecorator() {

  }

  void vec3dCurveOneDFunctionDecorator::buildPart(
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
    bool hasPoints = hasChild("Point_3", toBuildP);
    bool hasScaFunction = hasChild("function", toBuildP);
    

    if ( hasPoints && hasOrder ) {
			std::vector< dtPoint3 > pointsArray;
      QDomElement elementP = getChild("Point_3", toBuildP);
      //
      //set input
      //
      int order = getAttributeInt("order", toBuildP);
      while ( !elementP.isNull() ) {
				std::vector< dtPoint3 > workingPoint;
        this->createBasic( &elementP, bC, cValP, depSFunP, &workingPoint );
        for (int ii=0;ii<workingPoint.size();ii++) {
          // put in point builder
					pointsArray.push_back( workingPoint[ii] );
					//delete workingPoint[ii];
        }
      // next sibling
      elementP = QDomElement( getNextSibling("Point_3", elementP) );      
      }
			ptrHandling<dtCurve> dtC( 
				bSplineCurve_pointConstructOCC(pointsArray, order).result() 
			);

      //
      // create scaCurve2dOneD
      //
      sFunP->push_back( new vec3dCurveOneD( dtC.get() ) );			
    }
    else {
      dt__throw(
				buildPart(),
				<< dt__eval(hasOrder) << std::endl
				<< dt__eval(hasPoints) << std::endl
				<< dt__eval(hasScaFunction) 
			);
    }
  }
}
