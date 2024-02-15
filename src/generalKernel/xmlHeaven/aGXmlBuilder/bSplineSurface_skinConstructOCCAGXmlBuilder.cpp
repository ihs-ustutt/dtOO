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

#include "bSplineSurface_skinConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineSurface_skinConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineSurface_skinConstructOCCAGXmlBuilder, 
      new bSplineSurface_skinConstructOCCAGXmlBuilder()
    )
  );
  
  bSplineSurface_skinConstructOCCAGXmlBuilder
    ::bSplineSurface_skinConstructOCCAGXmlBuilder() {
  }

  bSplineSurface_skinConstructOCCAGXmlBuilder
    ::~bSplineSurface_skinConstructOCCAGXmlBuilder() {
  }

  void bSplineSurface_skinConstructOCCAGXmlBuilder::buildPart(
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

    vectorHandling< dtCurve const * > ccV;    
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticGeometry", toBuild), anEl
    ) {
      analyticGeometry * aG_t
      =
      dtXmlParserBase::createAnalyticGeometry(anEl, bC, cV, aF, aG);
      dt__ptrAss(
        analyticCurve const * s3, analyticCurve::ConstDownCast(aG_t)
      );
      ccV.push_back( s3->ptrConstDtCurve()->clone() );
    }

    dtInt nIter = 0;
    if ( dtXmlParserBase::hasAttribute("nIterations", toBuild) ) {
      nIter 
      = 
      dtXmlParserBase::getAttributeIntMuParse("nIterations", toBuild, cV, aF);
    }
    dtInt minDeg = 1;
    if ( dtXmlParserBase::hasAttribute("orderMin", toBuild) ) {
      minDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMin", toBuild, cV, aF);
    }    
    dtInt maxDeg = 25;
    if ( dtXmlParserBase::hasAttribute("orderMax", toBuild) ) {
      maxDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMax", toBuild, cV, aF);
    }
    
    dt__pH(dtSurface) dtS(
      bSplineSurface_skinConstructOCC(ccV, minDeg, maxDeg, nIter).result()
    );
              
    if ( 
      dtXmlParserBase::hasAttribute("exchange", toBuild)  
      && 
      dtXmlParserBase::getAttributeBool("exchange", toBuild)
    ) {
      dtS.reset( 
        bSplineSurface_exchangeSurfaceConstructOCC(dtS.get()).result()
      );
    }
    result->push_back( new analyticSurface( dtS.get() ) );
    ccV.destroy();
  }
}
