/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <baseContainerHeaven/baseContainer.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtOCCCurve2d.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleRatioDeltaYConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve2d_angleDeltaXDeltaYConstructOCC.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <jsonHeaven/aFJsonBuilder/bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder.h>
#include <logMe/dtMacros.h>
#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  bool bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder, 
      new bSplineCurve2d_3PointMeanlineConstructAFXmlBuilder()
    )
  );
  
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
		lvH_constValue const * const cV, 
		lvH_analyticFunction const * const aF,
		lvH_analyticFunction * result 
	) const {
		jsonPrimitive config;
    if ( dtXmlParserBase::hasAttribute("alpha_one", toBuildP) ) {
			config.append(
        "alphaOne", 
        dtXmlParserBase::getAttributeFloatMuParse(
          "alpha_one", toBuildP, cV, aF
        )
			);
		}
    if ( dtXmlParserBase::hasAttribute("alpha_two", toBuildP) ) {
			config.append(
        "alphaTwo", 
        dtXmlParserBase::getAttributeFloatMuParse(
          "alpha_two", toBuildP, cV, aF
        )
			);
		}
    if ( dtXmlParserBase::hasAttribute("ratio", toBuildP) ) {
			config.append(
        "ratio", 
        dtXmlParserBase::getAttributeFloatMuParse(
          "ratio", toBuildP, cV, aF
        )
			);
		}

		if ( dtXmlParserBase::hasAttribute("delta_x", toBuildP) ) {
  		config.append(
        "deltaX", 
        dtXmlParserBase::getAttributeFloatMuParse(
          "delta_x", toBuildP, cV, aF
        )
			);
		}

    if ( dtXmlParserBase::hasAttribute("delta_y", toBuildP) ) {
  		config.append(
        "deltaY", 
        dtXmlParserBase::getAttributeFloatMuParse(
          "delta_y", toBuildP, cV, aF
        )
			);
		}

    if ( 
      dtXmlParserBase::hasAttribute("targetLength", toBuildP)
			&&
      dtXmlParserBase::hasAttribute("targetLengthTolerance", toBuildP)
    ) {
      config.append(
				"targetLength",
			  dtXmlParserBase::getAttributeFloatMuParse(
          "targetLength", toBuildP, cV, aF
        )
			);
      config.append(
				"targetLengthTolerance",
			  dtXmlParserBase::getAttributeFloatMuParse(
          "targetLengthTolerance", toBuildP, cV, aF
        )
			);
    } 
    
    if ( 
      dtXmlParserBase::hasAttribute("originOnLengthPercent", toBuildP) 
    ) {
      config.append(
        "originOnLengthPercent",
				dtXmlParserBase::getAttributeFloatMuParse(
          "originOnLengthPercent", toBuildP, cV, aF
        )
		  );
    } 
		if ( dtXmlParserBase::hasAttribute("revert", toBuildP) ) {
			config.append(
			  "revert",
   			dtXmlParserBase::getAttributeBool("revert", toBuildP) 
			);
		}
		dt__debug(buildPart(), << "config = " << config.toStdString());
		dt__forAllRefAuto( 
  	  bSplineCurve2d_3PointMeanlineConstructAFJsonBuilder().buildPart(
	  	  config, bC, cV, aF
		  ),
			aPart 
		) {
			result->push_back( aPart );
		}
  }
}
