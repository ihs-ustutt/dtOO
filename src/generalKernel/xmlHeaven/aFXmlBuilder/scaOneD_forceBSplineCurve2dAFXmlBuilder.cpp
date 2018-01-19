#include "scaOneD_forceBSplineCurve2dAFXmlBuilder.h"
#include "xmlHeaven/dtXmlParser.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_pointConstructOCC.h>
#include <geometryEngine/dtCurve2d.h>
#include <analyticFunctionHeaven/scaCurve2dOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <boost/assign.hpp>

namespace dtOO {
  scaOneD_forceBSplineCurve2dAFXmlBuilder
    ::scaOneD_forceBSplineCurve2dAFXmlBuilder() {
  }

  scaOneD_forceBSplineCurve2dAFXmlBuilder
    ::~scaOneD_forceBSplineCurve2dAFXmlBuilder() {

  }
  
  std::vector< std::string > scaOneD_forceBSplineCurve2dAFXmlBuilder
    ::factoryAlias( void ) const {
    return ::boost::assign::list_of("scaCurve2dOneD");
  }

  void scaOneD_forceBSplineCurve2dAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		cVPtrVec const * const cValP, 
		aFPtrVec const * const depSFunP,
		aFPtrVec * sFunP 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("order", toBuildP)
      ||
      !dtXmlParserBase::hasChild("Point_2", toBuildP),
      buildPart()
    );

    std::vector< dtPoint2 > pointsArray;
    ::QDomElement elementP = dtXmlParserBase::getChild("Point_2", toBuildP);
    //
    //set input
    //
    int order 
    = 
    dtXmlParserBase::getAttributeIntMuParse("order", toBuildP, cValP);
    while ( !elementP.isNull() ) {
      std::vector< dtPoint2 > workingPoint;
      dtXmlParserBase::dtXmlParserBase::createBasic( 
        &elementP, bC, cValP, depSFunP, &workingPoint 
      );
      for (int ii=0; ii<workingPoint.size();ii++) {
        pointsArray.push_back( workingPoint[ii] );
//					delete workingPoint[ii];
      }
    // next sibling
    elementP 
    = 
    ::QDomElement( dtXmlParserBase::getNextSibling("Point_2", elementP) );      
    }
    ptrHandling<dtCurve2d> dtC2d( 
      bSplineCurve2d_pointConstructOCC(pointsArray, order).result() 
    );

    //
    // create scaCurve2dOneD
    //
    sFunP->push_back( new scaCurve2dOneD( dtC2d.get() ) );
  }
}
