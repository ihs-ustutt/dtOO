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

#include "bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_bSplineCurveSplitConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder, 
      new bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder()
    )
  );
  
  bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder
    ::bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder
    ::~bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_bSplineCurveSplitConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
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
      analyticCurve const * const sC, 
      analyticCurve::ConstDownCast( toSplit.get() )
    );
    
    //
    // u0
    //
    dtReal u0;
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
    dtReal u1;
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
      new analyticCurve(
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
