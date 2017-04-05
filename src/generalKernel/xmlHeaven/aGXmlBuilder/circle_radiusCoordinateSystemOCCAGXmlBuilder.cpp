#include "circle_radiusCoordinateSystemOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/circle_radiusCoordinateSystemConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  circle_radiusCoordinateSystemOCCAGXmlBuilder
    ::circle_radiusCoordinateSystemOCCAGXmlBuilder() {
  }

  circle_radiusCoordinateSystemOCCAGXmlBuilder
    ::~circle_radiusCoordinateSystemOCCAGXmlBuilder() {
  }

  void circle_radiusCoordinateSystemOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
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
    float const rr
    = 
    dtXmlParserBase::getAttributeFloatMuParse("radius", toBuild, cV, aF);
       
    //
    // origin
    //
    ::QDomElement wE = dtXmlParserBase::getChild("Point_3", toBuild);
    dtPoint3 pp = dtXmlParserBase::createDtPoint3( &wE, bC, cV, aF, aG );
    
    //
    // main direction ( x axis )
    //
    wE = dtXmlParserBase::getChild("Vector_3", toBuild);
    dtVector3 xx = dtXmlParserBase::createDtVector3( &wE, bC, cV, aF, aG );
    
    dt__pH(dtCurve) dtC(
      circle_radiusCoordinateSystemConstructOCC(pp, xx, rr).result()
    );
    if ( dtXmlParserBase::hasAttribute("angle", toBuild) ) {
      //
      // get angle
      //
      float angle 
      = 
      dtXmlParserBase::getAttributeFloatMuParse("angle", toBuild, cV, aF);
      
      //
      // calculate uMax
      //
      float uMax 
      = 
      dtC->getUMin() + (dtC->getUMax() - dtC->getUMin()) * angle/(2.*M_PI);
      
      //
      // trim curve
      //
      dtC.reset( 
        trimmedCurve_uBounds(dtC.get(), dtC->getUMin(), uMax).result() 
      );
    }
    
    result->push_back( new splineCurve3d( dtC.get() ) );
  }
}