#include "bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <QtXml/QDomElement>

#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC.h>

namespace dtOO {
  bool bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder, 
      new bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder()
    )
  );
  
  bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder
    ::bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder() { 
  }

  bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder
    ::~bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder() {

  }

  void bSplineCurve2d_5PointMeanlineConstructAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		lvH_constValue const * const cValP, 
		lvH_analyticFunction const * const depSFunP,
    lvH_analyticFunction * sFunP 		
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("alpha_one", toBuildP), buildPart()
    );
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("alpha_two", toBuildP), buildPart()
    );    
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("point_one", toBuildP), buildPart()
    );    
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("point_two", toBuildP), buildPart()
    );        
    bool hasRatio = dtXmlParserBase::hasAttribute("ratio", toBuildP);
		bool hasDeltaX = dtXmlParserBase::hasAttribute("delta_x", toBuildP);
    bool hasDeltaY = dtXmlParserBase::hasAttribute("delta_y", toBuildP);

    dtReal alphaOne 
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
    dtReal alphaTwo 
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
    dtReal pointOne 
    = 
    dtXmlParserBase::muParseString(
      dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr(
          "point_one", 
          toBuildP
        ), 
        cValP, 
        depSFunP
      )
    ); 
    dtReal pointTwo 
    = 
    dtXmlParserBase::muParseString(
      dtXmlParserBase::replaceDependencies(
        dtXmlParserBase::getAttributeStr(
          "point_two", 
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
      dtReal deltaY 
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceDependencies(
					dtXmlParserBase::getAttributeStr(
						"delta_y", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			); 
      dtReal ratio
			= 
			dtXmlParserBase::muParseString(
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
				bSplineCurve2d_angleRatioDeltaYPointOnePointTwoConstructOCC(
					alphaOne, alphaTwo, ratio, deltaY, pointOne, pointTwo
				).result()
			);
    }
    else if ( !hasRatio && hasDeltaX && hasDeltaY ) {    
      //
      // get necessary values
      //
      dtReal deltaX 
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceDependencies(
					dtXmlParserBase::getAttributeStr(
						"delta_x", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			); 
      dtReal deltaY 
			= 
			dtXmlParserBase::muParseString(
				dtXmlParserBase::replaceDependencies(
					dtXmlParserBase::getAttributeStr(
						"delta_y", 
						toBuildP
					), 
					cValP, 
					depSFunP
				)
			); 
			dtC2d.reset(
				bSplineCurve2d_angleDeltaXDeltaYPointOnePointTwoConstructOCC(
					alphaOne, alphaTwo, deltaX, deltaY, pointOne, pointTwo
				).result()
			);      
    }
    else dt__throwUnexpected(buildPart());
    
		if ( dtXmlParserBase::hasAttribute("revert", toBuildP) ) {
			if ( dtXmlParserBase::getAttributeBool("revert", toBuildP) ) {
				dtOCCCurve2d::SecureCast(dtC2d.get())->revert();
			}
		}
		sFunP->push_back( new vec2dCurve2dOneD( dtC2d.get() ) );
  }
}
