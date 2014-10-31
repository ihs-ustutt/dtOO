#include "scaCurve2dOneDFunctionDecorator.h"
#include <dtTransformerHeaven/doNothing.h>
#include <functionHeaven/analyticFunction.h>
#include <dtTransformerHeaven/dtTransformerFactory.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <functionHeaven/scaCurve2dOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  scaCurve2dOneDFunctionDecorator::scaCurve2dOneDFunctionDecorator() {
  }

  scaCurve2dOneDFunctionDecorator::~scaCurve2dOneDFunctionDecorator() {

  }

  void scaCurve2dOneDFunctionDecorator::buildPart(
         QDomElement const & toBuildP, 
         vectorHandling< constValue * > const * const cValP, 
         vectorHandling< analyticFunction * > const * const depSFunP,
         vectorHandling< analyticFunction * > * sFunP ) const {
    //
    //check input
    //
    bool hasOrder = hasAttribute("order", toBuildP);
    bool hasPoints = hasChild("Point_2", toBuildP);
    bool hasScaFunction = hasChild("function", toBuildP);
    bool hasShapeFactor = hasAttribute("shape_factor", toBuildP);
    
    if (hasShapeFactor && hasOrder) {
      dt__THROW(buildPart(),
              << "order- and shape_factor-Attribute given.");
    }

    /* ------------------------------------------------------------------------ */
    // create spline out of points and order
    /* ------------------------------------------------------------------------ */
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
      sFunP->push_back( new scaCurve2dOneD( dtC2d.get() ) );			
    }
    /* ------------------------------------------------------------------------ */
    // create conic arc spline out of points and a shape factor
    /* ------------------------------------------------------------------------ */
//    else if ( hasPoints && hasShapeFactor ) {
//      std::vector< dtPoint2* > workingPoint;
//      QDomElement elementP = QDomElement( getChild("Point_2", toBuildP) );
//      //
//      //set input
//      //
//      float shapeFactor = 
//      muParseString(
//        replaceUsedFunctions(
//          getAttributeStr(
//            "shape_factor", 
//            toBuildP
//          ),
//          cValP,
//          depSFunP
//        )
//      );
//      sFunBuilderP->setAttribute( &shapeFactor );      
//      while ( !elementP.isNull() ) {
//        this->createBasic( &elementP, cValP, depSFunP, &workingPoint );
//        for (int ii=0;ii<workingPoint.size();ii++) {
//          // put in point builder
//          sFunBuilderP->setAttribute(workingPoint[ii]);
//        }
//        for (int ii=0;ii<workingPoint.size();ii++) {
//          delete workingPoint[ii];
//        }      
//        workingPoint.clear();      
//      //goto next sibling
//      elementP = QDomElement( getNextSibling("Point_2", elementP) );      
//      }
//    }    
    /* ------------------------------------------------------------------------ */
    // copy an existing scaFunction
    /* ------------------------------------------------------------------------ */  
    else if ( hasScaFunction ) {
      QDomElement el = getChild("function", toBuildP);
			analyticFunction const * const aF 
			= 
			depSFunP->get( getAttributeStr("label", el) );
			sFunP->push_back( aF->clone() );
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasPoints) << LOGDEL
              << DTLOGEVAL(hasScaFunction) );
    }

		//
		// transform
		//
    ptrHandling< dtTransformer > cTransP(  
		  createTransformer(&toBuildP, cValP, depSFunP)
		);	
    if ( cTransP->isNecessary() ) {
      *sFunP = cTransP->apply(sFunP);
    }
  }
}
