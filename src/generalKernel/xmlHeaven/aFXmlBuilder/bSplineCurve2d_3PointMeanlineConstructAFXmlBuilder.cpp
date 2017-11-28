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
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("alpha_one", toBuildP)
      ||
      !dtXmlParserBase::hasAttribute("alpha_two", toBuildP), 
      buildPart()
    );        
    bool hasRatio = dtXmlParserBase::hasAttribute("ratio", toBuildP);
		bool hasDeltaX = dtXmlParserBase::hasAttribute("delta_x", toBuildP);
    bool hasDeltaY = dtXmlParserBase::hasAttribute("delta_y", toBuildP);

    float alphaOne 
    = 
    dtXmlParserBase::getAttributeFloatMuParse(
      "alpha_one", toBuildP, cV, aF
    );      
    float alphaTwo 
    = 
    dtXmlParserBase::getAttributeFloatMuParse(
      "alpha_two", toBuildP, cV, aF
    );           
    //
    //
    //
		dt__pH(dtCurve2d) dtC2d;
    if ( hasRatio && !hasDeltaX && hasDeltaY ) {
      //
      // get necessary values
      //
      float deltaY 
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "delta_y", toBuildP, cV, aF
      );   
      float ratio
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "ratio", toBuildP, cV, aF
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
      float deltaY 
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "delta_y", toBuildP, cV, aF
      );   
      float deltaX 
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "delta_x", toBuildP, cV, aF
      );   			
			dtC2d.reset(
				bSplineCurve2d_angleDeltaXDeltaYConstructOCC(
					alphaOne, alphaTwo, deltaX, deltaY
				).result()
			);
    }		
    else dt__throwUnexpected(buildPart());

    if ( 
      dtXmlParserBase::hasAttribute("targetLength", toBuildP) 
    ) {
      float tL
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "targetLength", toBuildP, cV, aF
      );
      float tLTol
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "targetLengthTolerance", toBuildP, cV, aF
      );      
      logContainer<bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder> logC(
        logINFO, "buildPart()"
      );      
      logC() << "Detect targetLength" << std::endl;
      dt__forFromToIndex(0, 99, ii) {
        float scale = tL / dtC2d->length();
      
        dtPoint2 p0 = dtC2d->controlPoint(0);
        dtVector2 v0 = dtC2d->controlPoint(1) - dtC2d->controlPoint(0);
        dtVector2 v1 = dtC2d->controlPoint(2) - dtC2d->controlPoint(1);

        dtC2d->setControlPoint( 1, p0 + ( 1. + (scale-1.)/2. ) * v0 );
        dtC2d->setControlPoint( 2, p0 + ( 1. + (scale-1.)/2. ) * ( v0 + v1 ) );
        
        logC() 
          << logMe::dtFormat("( %3d ) : %5.2f -> %5.2f") 
            % ii % scale % (tL / dtC2d->length())
          << std::endl;
        if ( fabs(tL / dtC2d->length() - 1.) <= tLTol ) break;
      }
    } 
    
    if ( 
      dtXmlParserBase::hasAttribute("originOnLengthPercent", toBuildP) 
    ) {
      float lP 
      =
      dtXmlParserBase::getAttributeFloatMuParse(
        "originOnLengthPercent", toBuildP, cV, aF
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
		result->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );
  }
}
