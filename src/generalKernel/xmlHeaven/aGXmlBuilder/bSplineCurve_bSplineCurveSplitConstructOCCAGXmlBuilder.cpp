#include "bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_bSplineCurveSplitConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder
    ::bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder
    ::~bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), 
      buildPart()
    );
    
    dt__pH(analyticGeometry) toSplit(
      dtXmlParserBase::createAnalyticGeometry( 
        dtXmlParserBase::getChild("analyticGeometry", toBuild), 
        bC, 
        cV, 
        aF, 
        aG
      )
    );
    
    dt__ptrAss(
      splineCurve3d const * const sC, 
      splineCurve3d::ConstDownCast( toSplit.get() )
    );
    
    //
    // u0
    //
    float u0;
    if(
      dtXmlParserBase::hasAttribute("parameter_percent_one_from", toBuild)
    ) {    
      u0 
      = 
      sC->u_percent(        
        dtXmlParserBase::getAttributeFloatMuParse(
          "parameter_percent_one_from", toBuild, cV
        )
      );
    }
    else if(
      dtXmlParserBase::hasAttribute("length_percent_from", toBuild)
    ) {    
      u0 
      = 
      sC->u_lPercent(        
        dtXmlParserBase::getAttributeFloatMuParse(
          "length_percent_from", toBuild, cV
        )
      );
    } 
    else if(
      dtXmlParserBase::hasAttribute("parameter_one_from", toBuild)
    ) {    
      u0 
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "parameter_one_from", toBuild, cV
      );
    }
    else dt__throwUnexpected(buildPart());
    
    //
    // u1
    //
    float u1;
    if(
      dtXmlParserBase::hasAttribute("parameter_percent_one_to", toBuild)
    ) {    
      u1 
      = 
      sC->u_percent(        
        dtXmlParserBase::getAttributeFloatMuParse(
          "parameter_percent_one_to", toBuild, cV
        )
      );
    }
    else if(
      dtXmlParserBase::hasAttribute("length_percent_to", toBuild)
    ) {    
      u1 
      = 
      sC->u_lPercent(        
        dtXmlParserBase::getAttributeFloatMuParse(
          "length_percent_to", toBuild, cV
        )
      );
    } 
    else if(
      dtXmlParserBase::hasAttribute("parameter_one_to", toBuild)
    ) {    
      u1 
      = 
      dtXmlParserBase::getAttributeFloatMuParse(
        "parameter_one_to", toBuild, cV
      );
    }
    else dt__throwUnexpected(buildPart());
    
    dt__info(
      buildPart(),
      << "Split curve at:" << std::endl
      << dt__eval(u0) << std::endl
      << dt__eval(u1)
    );
    result->push_back( 
      new splineCurve3d(
        dt__tmpPtr(
          dtCurve, 
          bSplineCurve_bSplineCurveSplitConstructOCC(
            sC->ptrConstDtCurve(), u0, u1
          ).result()
        )
      )
    );
  }
}