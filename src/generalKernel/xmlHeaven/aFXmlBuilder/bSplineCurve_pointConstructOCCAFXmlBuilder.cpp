#include "bSplineCurve_pointConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/dtXmlParser.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  bSplineCurve_pointConstructOCCAFXmlBuilder
    ::bSplineCurve_pointConstructOCCAFXmlBuilder() {
  }

  bSplineCurve_pointConstructOCCAFXmlBuilder
    ::~bSplineCurve_pointConstructOCCAFXmlBuilder() {

  }

  std::vector< std::string > bSplineCurve_pointConstructOCCAFXmlBuilder
    ::factoryAlias( void ) const {
    return ::boost::assign::list_of
      ("vec3dCurveOneD")
      ("vec3dCurveOneDAFXmlBuilder");
  }  

  void bSplineCurve_pointConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		cVPtrVec const * const cValP, 
		aFPtrVec const * const depSFunP,
		aFPtrVec * sFunP 
	) const {
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuildP), buildPart());    
      //
      // get points
      //
      std::vector< dtPoint3 > points;
      dt__forAllRefAuto(
        dtXmlParserBase::getChildVector("Point_3", toBuildP), anEl
      ) {
        std::vector< dtPoint3 > wP;
        dtXmlParserBase::dtXmlParserBase::createBasic( 
          &anEl, bC, cValP, depSFunP, &wP 
        );
        dt__forAllRefAuto(wP, aWP) points.push_back( aWP );
      }
    
      //
      // construct by control points
      //
      ptrHandling<dtCurve> dtC;
      if ( 
        ( dtXmlParser::getAttributeStr("via", toBuildP, "") == "" )
        ||
        ( 
          dtXmlParser::getAttributeStr("via", toBuildP, "") 
          == 
          "pointConstruct" 
        )
      ) {
        dt__throwIf( 
          !dtXmlParserBase::hasAttribute("order", toBuildP), buildPart() 
        );
        dtC.reset( 
          bSplineCurve_pointConstructOCC(
            points, 
            dtXmlParserBase::getAttributeInt("order", toBuildP)
          ).result() 
        );
      }
      //
      // construct by pointInterpolate
      //
      else if (
        dtXmlParser::getAttributeStr("via", toBuildP, "") 
        == 
        "pointInterpolateConstruct" 
      ) {
        dt__throwIf( 
          dtXmlParserBase::hasAttribute("order", toBuildP), buildPart() 
        );
        dt__throwIf( 
          !dtXmlParserBase::hasAttribute("orderMin", toBuildP), buildPart() 
        );        
        dt__throwIf( 
          !dtXmlParserBase::hasAttribute("orderMax", toBuildP), buildPart() 
        );                
        dtC.reset( 
          bSplineCurve_pointInterpolateConstructOCC(
            points,
            dtXmlParserBase::getAttributeInt("orderMin", toBuildP),
            dtXmlParserBase::getAttributeInt("orderMax", toBuildP)          
          ).result() 
        );        
      }
      else dt__throwUnexpected(buildPart());

      //
      // create vec3dCurveOneD
      //
      sFunP->push_back( new vec3dCurveOneD( dtC.get() ) );			
  }
}
