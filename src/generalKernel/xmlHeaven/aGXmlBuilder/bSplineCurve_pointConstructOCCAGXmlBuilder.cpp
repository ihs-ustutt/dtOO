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

#include "bSplineCurve_pointConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve_pointConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_pointConstructOCCAGXmlBuilder, 
      new bSplineCurve_pointConstructOCCAGXmlBuilder()
    )
  );
  
  bSplineCurve_pointConstructOCCAGXmlBuilder
    ::bSplineCurve_pointConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_pointConstructOCCAGXmlBuilder
    ::~bSplineCurve_pointConstructOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
	) const {
    //
		// check input
		//  
    dt__throwIf(!dtXmlParserBase::hasChild("Point_3", toBuild), buildPart());
    dt__throwIf(!dtXmlParserBase::hasAttribute("order", toBuild), buildPart());

    dtInt const order 
    = 
    dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV, aF);
    std::vector< dtPoint3 > workingPointP;
    ::QDomElement wElement = dtXmlParserBase::getChild("Point_3", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::createBasic( &wElement, bC, cV, aF, aG, &workingPointP );
      wElement = dtXmlParserBase::getNextSibling("Point_3", wElement);
    }
    result->push_back( 
      new analyticCurve(
        dt__tmpPtr(
          dtCurve, 
          bSplineCurve_pointConstructOCC(workingPointP, order).result()
        )
      )
    );
  }
}
