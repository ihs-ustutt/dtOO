#include "geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <xmlHeaven/dtXmlParser.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/geomFillBoundWithSurf_surfaceConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec2dCurve2dOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>

namespace dtOO {
  bool geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder, 
      new geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder()
    )
  );
  
  geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder
    ::geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder() {
  }

  geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder
    ::~geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder() {

  }

  void geomFillBoundWithSurf_surfaceConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuildP, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuildP), 
      buildPart()
    );  
    
    //
    // get analytic function
    //
    std::vector< ::QDomElement > elVec 
    = 
    dtXmlParserBase::getChildVector("analyticFunction", toBuildP);
    
    dt__ptrAss(
      vec3dSurfaceTwoD * dtS, 
      vec3dSurfaceTwoD::DownCast(
        dtXmlParserBase::createAnalyticFunction(
          elVec[0], bC, cV, aF
        )
      )
    );
    dt__ptrAss(
      vec2dCurve2dOneD * dtC2d, 
      vec2dCurve2dOneD::DownCast(
        dtXmlParserBase::createAnalyticFunction(
          elVec[1], bC, cV, aF
        )
      )
    );    

    //
    // create boundary curve
    //
    ptrHandling<dtCurve> dtC(
      geomFillBoundWithSurf_surfaceConstructOCC(
        dtS->ptrDtSurface(), dtC2d->ptrDtCurve2d()
      ).result()
    );
      
    //
    // push back to result
    //
    result->push_back( new vec3dCurveOneD( dtC.get() ) );			
  }
}
