#include "geomShape_readIgesOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/geomShape_readIgesOCC.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool geomShape_readIgesOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      geomShape_readIgesOCCAGXmlBuilder, 
      new geomShape_readIgesOCCAGXmlBuilder()
    )
  );
  
  geomShape_readIgesOCCAGXmlBuilder
    ::geomShape_readIgesOCCAGXmlBuilder() {
  }

  geomShape_readIgesOCCAGXmlBuilder
    ::~geomShape_readIgesOCCAGXmlBuilder() {
  }

  void geomShape_readIgesOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasAttribute("filename", toBuild), 
      buildPart()
    );

    geomShape_readIgesOCC readIges(
      dtXmlParserBase::getAttributeStr("filename", toBuild)
    );

    dt__forAllRefAuto(readIges.resultSurfaceRef(), aSurf) {
      result->push_back( new analyticSurface( &aSurf ) );
    }
    dt__forAllRefAuto(readIges.resultCurveRef(), aCurv) {
      result->push_back( new analyticCurve( &aCurv ) );
    }
  }
}