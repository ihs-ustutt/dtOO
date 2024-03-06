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

#include "bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructArcLengthParaOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder, 
      new bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder()
    )
  );
  
  bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder
    ::bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder() {
  }

  bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder
    ::~bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder() {
  }

  void bSplineCurve_pointConstructArcLengthParaOCCAGXmlBuilder::buildPart(
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
    dt__throwIf(!dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("tolerance", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("max_order", toBuild), buildPart());
		dt__throwIf(!dtXmlParserBase::hasAttribute("num_segments", toBuild), buildPart());

    dtInt const maxOrder = dtXmlParserBase::getAttributeIntMuParse("max_order", toBuild, cV, aF);
    dtInt const nSegments 
    = 
    dtXmlParserBase::getAttributeIntMuParse("num_segments", toBuild, cV, aF);
    dtReal const tol 
    = 
    dtXmlParserBase::getAttributeFloatMuParse("tolerance", toBuild, cV, aF);

    ::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    dt__pH(analyticGeometry) aG_t( 
      dtXmlParserBase::createAnalyticGeometry(&wElement, bC, cV, aF, aG)
    );
    dt__ptrAss(
      analyticCurve const * sC3, 
      analyticCurve::ConstDownCast(aG_t.get())
    );
    result->push_back( 
      new analyticCurve(
        bSplineCurve_pointConstructArcLengthParaOCC(
          sC3->ptrConstDtCurve(), tol, maxOrder, nSegments
        ).result()
      )
    );
  }
}
