#include "bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include "analyticGeometryHeaven/analyticCurve.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/dtXmlParser.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <geometryEngine/geoBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>

namespace dtOO {
  bool 
  bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder, 
      new bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder()
    )
  );
  
  bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder
    ::bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder() {
  }

  bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder
    ::~bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder() {

  }

  void bSplineCurve_poleWeightKnotMultOrderConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild, 
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aGPtrVec const * const aG,        
    aGPtrVec * result
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("Point_3", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("weights|knots|mults|order", toBuild),
      buildPart()
    );    
      //
      // get points
      //
      std::vector< dtPoint3 > poles;
      dt__forAllRefAuto(
        dtXmlParserBase::getChildVector("Point_3", toBuild), anEl
      ) {
        std::vector< dtPoint3 > wP;
        dtXmlParserBase::dtXmlParserBase::createBasic( 
          &anEl, bC, cV, aF, aG, &wP 
        );
        dt__forAllRefAuto(wP, aWP) poles.push_back( aWP );
      }

      //
      // parse weights, knots, mults, order and call geoBuilder
      //
      dt__pH(dtCurve) dtC( 
        bSplineCurve_poleWeightKnotMultOrderConstructOCC(
          poles, 
          dtXmlParserBase::getAttributeFloatVectorMuParse(
            "weights", toBuild, cV, aF
          ), 
          dtXmlParserBase::getAttributeFloatVectorMuParse(
            "knots", toBuild, cV, aF
          ),
          dtXmlParserBase::getAttributeIntVectorMuParse(
            "mults", toBuild, cV, aF
          ), 
          dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV),
          dtXmlParserBase::getAttributeBool("periodic", toBuild, false)
        ).result() 
      );        
      
      //
      // create vec3dCurveOneD
      //
      result->push_back( new analyticCurve( dtC.get() ) );			
  }
}
