#include "bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/dtXmlParser.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineCurve_poleWeightKnotMultOrderConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>

namespace dtOO {
  bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder
    ::bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder() {
  }

  bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder
    ::~bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder() {

  }

  void bSplineCurve_poleWeightKnotMultOrderConstructOCCAFXmlBuilder::buildPart(
    ::QDomElement const & toBuild, 
    baseContainer * const bC,
    cVPtrVec const * const cV,
    aFPtrVec const * const aF,
    aFPtrVec * result
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("Point_3", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("weights", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("knots", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("mults", toBuild),
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
          &anEl, bC, cV, aF, &wP 
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
          dtXmlParserBase::getAttributeIntMuParse("order", toBuild, cV)
        ).result() 
      );        
      
      //
      // create vec3dCurveOneD
      //
      result->push_back( new vec3dCurveOneD( dtC.get() ) );			
  }
}
