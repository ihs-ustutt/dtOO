#include "bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_bSplineCurveSplitConstructOCC.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder, 
      new bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder()
    )
  );
  
  bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder
    ::bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder() {
  }

  bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder
    ::~bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder() {
  }

  void bSplineCurve_bSplineCurveSplitConstructOCCAFXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aFPtrVec * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuild), 
      buildPart()
    );
    
    dt__pH(analyticFunction) toSplit(
      dtXmlParserBase::createAnalyticFunction( 
        dtXmlParserBase::getChild("analyticFunction", toBuild), 
        bC, 
        cV, 
        aF
      )
    );
    
    dt__ptrAss(
      vec3dCurveOneD const * const v3d, 
      vec3dCurveOneD::ConstDownCast( toSplit.get() )
    );
    
    //
    // u0
    //
    float x0
    = 
    dtXmlParserBase::getAttributeFloatMuParse(
      "x_one_from", toBuild, cV, aF
    );
    
    //
    // u1
    //
    float x1
    = 
    dtXmlParserBase::getAttributeFloatMuParse(
      "x_one_to", toBuild, cV, aF
    );

    
    dt__info(
      buildPart(),
      << "Split curve at:" << std::endl
      << dt__eval(x0) << std::endl
      << dt__eval(x1)
    );
    result->push_back( 
      new vec3dCurveOneD(
        dt__tmpPtr(
          dtCurve, 
          bSplineCurve_bSplineCurveSplitConstructOCC(
            v3d->ptrConstDtCurve(), x0, x1
          ).result()
        )
      )
    );
  }
}