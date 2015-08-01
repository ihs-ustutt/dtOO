#include "scaCurve2dOneDAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  scaCurve2dOneDAFXmlBuilder::scaCurve2dOneDAFXmlBuilder() {
  }

  scaCurve2dOneDAFXmlBuilder::~scaCurve2dOneDAFXmlBuilder() {

  }

  void scaCurve2dOneDAFXmlBuilder::buildPart(
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
    bool hasScaFunction = dtXmlParserBase::hasChild("function", toBuildP);
    bool hasShapeFactor = dtXmlParserBase::hasAttribute("shape_factor", toBuildP);
    
    if (hasShapeFactor && hasOrder) {
      dt__throw(buildPart(),
              << "order- and shape_factor-Attribute given.");
    }

    /* ------------------------------------------------------------------------ */
    // create spline out of points and order
    /* ------------------------------------------------------------------------ */
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
        for (int ii=0; ii<workingPoint.size();ii++) {
					pointsArray.push_back( workingPoint[ii] );
//					delete workingPoint[ii];
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
      sFunP->push_back( new scaCurve2dOneD( dtC2d.get() ) );			
    }
    /* ------------------------------------------------------------------------ */
    // create conic arc spline out of points and a shape factor
    /* ------------------------------------------------------------------------ */
//    else if ( hasPoints && hasShapeFactor ) {
//      std::vector< dtPoint2* > workingPoint;
//      ::QDomElement elementP = ::QDomElement( dtXmlParserBase::getChild("Point_2", toBuildP) );
//      //
//      //set input
//      //
//      float shapeFactor = 
//      dtXmlParserBase::muParseString(
//        dtXmlParserBase::replaceDependencies(
//          dtXmlParserBase::getAttributeStr(
//            "shape_factor", 
//            toBuildP
//          ),
//          cValP,
//          depSFunP
//        )
//      );
//      sFunBuilderP->setAttribute( &shapeFactor );      
//      while ( !elementP.isNull() ) {
//        this->dtXmlParserBase::createBasic( &elementP, cValP, depSFunP, &workingPoint );
//        for (int ii=0;ii<workingPoint.size();ii++) {
//          // put in point builder
//          sFunBuilderP->setAttribute(workingPoint[ii]);
//        }
//        for (int ii=0;ii<workingPoint.size();ii++) {
//          delete workingPoint[ii];
//        }      
//        workingPoint.clear();      
//      //goto next sibling
//      elementP = ::QDomElement( dtXmlParserBase::getNextSibling("Point_2", elementP) );      
//      }
//    }    
    /* ------------------------------------------------------------------------ */
    // copy an existing scaFunction
    /* ------------------------------------------------------------------------ */  
    else if ( hasScaFunction ) {
      ::QDomElement el = dtXmlParserBase::getChild("function", toBuildP);
			analyticFunction const * const aF 
			= 
			depSFunP->get( dtXmlParserBase::getAttributeStr("label", el) );
			sFunP->push_back( aF->clone() );
    }
    else {
      dt__throw(buildPart(),
              << dt__eval(hasOrder) << std::endl
              << dt__eval(hasPoints) << std::endl
              << dt__eval(hasScaFunction) );
    }
  }
}
