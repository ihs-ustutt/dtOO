#include "ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/ellipse_radiusRadiusCoordinateSystemConstructOCC.h>
#include <geometryEngine/geoBuilder/trimmedCurve_uBounds.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder
    ::ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder() {
  }

  ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder
    ::~ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder() {
  }

  void ellipse_radiusRadiusCoordinateSystemOCCAGXmlBuilder::buildPart(
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
      !dtXmlParserBase::hasAttribute("radius_one", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("radius_two", toBuild),
      buildPart()
    );

    //
    // radius
    //
    float const r0
    = 
    dtXmlParserBase::getAttributeFloatMuParse("radius_one", toBuild, cV, aF, aG);
    float const r1
    = 
    dtXmlParserBase::getAttributeFloatMuParse("radius_two", toBuild, cV, aF, aG);
    dt__info(
      buildPart(), 
      << "r0 = " << r0 << std::endl
      << "r1 = " << r1      
    );
    dt__throwIf( r0<r1, buildPart() );
    
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
        ellipse_radiusRadiusCoordinateSystemConstructOCC(
          pp, xx, r0, r1
        ).result()
      );
    }
    else if ( wE_v.size() == 2 ) {
      dtVector3 nn = dtXmlParserBase::createDtVector3( &(wE_v[0]), bC, cV, aF, aG );
      dtVector3 xx = dtXmlParserBase::createDtVector3( &(wE_v[1]), bC, cV, aF, aG );      
      
      dtC.reset(
        ellipse_radiusRadiusCoordinateSystemConstructOCC(
          pp, nn, xx, r0, r1
        ).result()
      );      
    }
    else dt__throwUnexpected(buildPart());
    
    if ( dtXmlParserBase::hasAttribute("angle", toBuild) ) {
      //
      // get angle
      //
      float angle 
      = 
      dtXmlParserBase::getAttributeFloatMuParse("angle", toBuild, cV, aF, aG);
      
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
    
    result->push_back( new analyticCurve( dtC.get() ) );
  }
}