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
    lvH_constValue const * const cV,           
    lvH_analyticFunction const * const aF,    
    lvH_analyticGeometry const * const aG,
    lvH_analyticGeometry * result 
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