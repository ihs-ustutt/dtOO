#include "bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder.h"

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>

#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder
    ::bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder() { 
  }

  bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder
    ::~bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder() {

  }

  std::vector< std::string > bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder
    ::factoryAlias( void ) const {
    return ::boost::assign::list_of("sca3PointMeanline");
  }
  
  void bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		cVPtrVec const * const cValP, 
		aFPtrVec const * const depSFunP,
		aFPtrVec * sFunP 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("alpha_one", toBuildP), buildPart()
    );
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("alpha_two", toBuildP), buildPart()
    );        
    bool hasRatio = dtXmlParserBase::hasAttribute("ratio", toBuildP);
		bool hasDeltaX = dtXmlParserBase::hasAttribute("delta_x", toBuildP);
    bool hasDeltaY = dtXmlParserBase::hasAttribute("delta_y", toBuildP);

    float alphaOne 
    = 
    dtXmlParserBase::muParseString(
      dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr(
          "alpha_one", 
          toBuildP
        ), 
        cValP, 
        depSFunP
      )
    );      
    float alphaTwo 
    = 
    dtXmlParserBase::muParseString(
      dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr(
          "alpha_two", 
          toBuildP
        ), 
        cValP, 
        depSFunP
      )
    );          
    //
    //
    //
		dt__pH(dtCurve2d) dtC2d;
    if ( hasRatio && !hasDeltaX && hasDeltaY ) {
      //
      // get necessary values
      //
      float deltaY = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float ratio = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "ratio", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     );      
			dtC2d.reset(
				bSplineCurve2d_angleRatioDeltaYConstructOCC(
					alphaOne, alphaTwo, ratio, deltaY
				).result()
			);
    }
    else if ( !hasRatio && hasDeltaX && hasDeltaY ) {
      //
      // get necessary values
      //
      float deltaY = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "delta_y", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 
      float deltaX = dtXmlParserBase::muParseString(
                       dtXmlParserBase::replaceDependencies(
                         dtXmlParserBase::getAttributeStr(
                           "delta_x", 
                           toBuildP
                         ), 
                         cValP, 
                         depSFunP
                       )
                     ); 			
			dtC2d.reset(
				bSplineCurve2d_angleDeltaXDeltaYConstructOCC(
					alphaOne, alphaTwo, deltaX, deltaY
				).result()
			);
    }		
    else dt__throwUnexpected(buildPart());
    
    if ( 
      dtXmlParserBase::hasAttribute("originOnLengthPercent", toBuildP) 
    ) {
      float lP 
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "originOnLengthPercent", toBuildP, cValP
      );
      dtPoint2 adjustP = dtC2d->point( dtC2d->u_lPercent( lP ) );
      dt__forFromToIndex(0, dtC2d->nControlPoints(), ii) {
        dtC2d->setControlPoint(
          ii,
          dtLinearAlgebra::toDtPoint2( dtC2d->controlPoint(ii) - adjustP )
        );
      }
    } 
		if ( dtXmlParserBase::hasAttribute("revert", toBuildP) ) {
			if ( dtXmlParserBase::getAttributeBool("revert", toBuildP) ) {
				dtOCCCurve2d::SecureCast(dtC2d.get())->revert();
			}
		}		
		sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );
  }
}
