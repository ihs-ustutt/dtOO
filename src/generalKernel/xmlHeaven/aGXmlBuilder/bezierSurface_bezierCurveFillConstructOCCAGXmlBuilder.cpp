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

#include "bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtSurface.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bezierSurface_bezierCurveFillConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder, 
      new bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder()
    )
  );
  
  bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder
    ::bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder() {
  }

  bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder
    ::~bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder() {
  }

  void bezierSurface_bezierCurveFillConstructOCCAGXmlBuilder::buildPart(
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
		
    lvH_analyticGeometry aG_t;
    ::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::createAdvanced(&wElement, bC, cV, aF, aG, &aG_t);
      wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);
    }

    vectorHandling< dtCurve const * > dtC( aG_t.size() );
    dt__forAllIndex(aG_t, ii) {
      dt__ptrAss(
        analyticCurve const * s3, analyticCurve::ConstDownCast(aG_t[ii])
      );
      dtC[ii] = s3->ptrConstDtCurve();
    }

    ptrHandling< dtSurface > dtS(
      bezierSurface_bezierCurveFillConstructOCC(dtC).result()
    );
    result->push_back( new analyticSurface(dtS.get()) );
  }
}
