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

#include "circle_radiusCoordinateSystemOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/circle_radiusCoordinateSystemConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool circle_radiusCoordinateSystemOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      circle_radiusCoordinateSystemOCCAGXmlBuilder, 
      new circle_radiusCoordinateSystemOCCAGXmlBuilder()
    )
  );
  
  circle_radiusCoordinateSystemOCCAGXmlBuilder
    ::circle_radiusCoordinateSystemOCCAGXmlBuilder() {
  }

  circle_radiusCoordinateSystemOCCAGXmlBuilder
    ::~circle_radiusCoordinateSystemOCCAGXmlBuilder() {
  }

  void circle_radiusCoordinateSystemOCCAGXmlBuilder::buildPart(
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
    dt__throwIf(
      !dtXmlParserBase::hasChild("Point_3", toBuild)
      ||
      !dtXmlParserBase::hasChild("Vector_3", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("radius", toBuild),
      buildPart()
    );

    //
    // radius
    //
    dtReal const rr
    = 
    dtXmlParserBase::getAttributeFloatMuParse("radius", toBuild, cV, aF, aG);
       
    //
    // origin
    //
    ::QDomElement wE = dtXmlParserBase::getChild("Point_3", toBuild);
    dtPoint3 pp = dtXmlParserBase::createDtPoint3( &wE, bC, cV, aF, aG );
    
    dt__pH(dtCurve) dtC;
    
    //
    // main direction ( x axis )
    //
    std::vector< ::QDomElement > wE_v 
    = 
    dtXmlParserBase::getChildVector("Vector_3", toBuild);
    
    if (wE_v.size() == 1) {
      dtVector3 xx = dtXmlParserBase::createDtVector3( &(wE_v[0]), bC, cV, aF, aG );
    
      dtC.reset(
        circle_radiusCoordinateSystemConstructOCC(pp, xx, rr).result()
      );
    }
    else if ( wE_v.size() == 2 ) {
      dtVector3 nn = dtXmlParserBase::createDtVector3( &(wE_v[0]), bC, cV, aF, aG );
      dtVector3 xx = dtXmlParserBase::createDtVector3( &(wE_v[1]), bC, cV, aF, aG );      
      
      dtC.reset(
        circle_radiusCoordinateSystemConstructOCC(pp, nn, xx, rr).result()
      );      
    }
    else dt__throwUnexpected(buildPart());
    
    if ( dtXmlParserBase::hasAttribute("angle", toBuild) ) {
      //
      // get angle
      //
      dtReal angle 
      = 
      dtXmlParserBase::getAttributeFloatMuParse("angle", toBuild, cV, aF, aG);
      
      //
      // calculate uMax
      //
      dtReal uMax 
      = 
      dtC->getUMin() + (dtC->getUMax() - dtC->getUMin()) * angle/(2.*M_PI);
      
      //
      // trim curve
      //
      dtC.reset( 
        trimmedCurve_uBounds(dtC.get(), dtC->getUMin(), uMax).result() 
      );
    }
    
    result->push_back( new analyticCurve( dtC.get() ) );
  }
}
