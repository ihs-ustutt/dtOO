#include "bSplineCurve2d_pointConstructOCCAFXmlBuilder.h"

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
  bSplineCurve2d_pointConstructOCCAFXmlBuilder
    ::bSplineCurve2d_pointConstructOCCAFXmlBuilder() {
  }

  bSplineCurve2d_pointConstructOCCAFXmlBuilder
    ::~bSplineCurve2d_pointConstructOCCAFXmlBuilder() {

  }

  void bSplineCurve2d_pointConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		cVPtrVec const * const cValP, 
		aFPtrVec const * const depSFunP,
		aFPtrVec * sFunP 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("Point_2", toBuildP)
      ||
      !dtXmlParserBase::hasAttribute("order", toBuildP), 
      buildPart()
    );    
    
    std::vector< dtPoint2 > pointsArray;
    ::QDomElement elementP = dtXmlParserBase::getChild("Point_2", toBuildP);
    //
    //set input
    //
    int order = dtXmlParserBase::getAttributeInt("order", toBuildP);
    while ( !elementP.isNull() ) {
      std::vector< dtPoint2 > workingPoint;
      dtXmlParserBase::dtXmlParserBase::createBasic( 
        &elementP, bC, cValP, depSFunP, &workingPoint 
      );
      for (int ii=0;ii<workingPoint.size();ii++) {
        // put in point builder
        pointsArray.push_back(workingPoint[ii]);
      }
    // next sibling
    elementP = dtXmlParserBase::getNextSibling("Point_2", elementP);
    }
    ptrHandling<dtCurve2d> dtC2d( 
      bSplineCurve2d_pointConstructOCC(pointsArray, order).result() 
    );

    //
    // create scaCurve2dOneD
    //
    sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );			
  }
}
