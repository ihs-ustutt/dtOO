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

#include "bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointInterpolateConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder, 
      new bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder()
    )
  );
  
  bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder
    ::bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder
    ::~bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointInterpolateConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
  ) const {
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuild), buildPart());

    std::vector< dtPoint3 > workingPointP;
    std::vector< ::QDomElement > wElementV
    = 
    dtXmlParserBase::getChildVector("Point_3", toBuild);
    
    dt__forAllRefAuto(wElementV, wElement) {
      dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &workingPointP );
    }
    
    dtInt minDeg = 3;
    if ( dtXmlParserBase::hasAttribute("orderMin", toBuild) ) {
      minDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMin", toBuild, cV, aF);
    }    
    dtInt maxDeg = 8;
    if ( dtXmlParserBase::hasAttribute("orderMax", toBuild) ) {
      maxDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMax", toBuild, cV, aF);
    }
    
    result->push_back( 
      new analyticCurve(
        bSplineCurve_pointInterpolateConstructOCC(workingPointP, minDeg, maxDeg).result()
      )
    );
  }
}
