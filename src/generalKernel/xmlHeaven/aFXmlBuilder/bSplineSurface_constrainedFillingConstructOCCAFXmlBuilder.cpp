#include "bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/aFXmlBuilderFactory.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <geometryEngine/geoBuilder/bSplineSurface_constrainedFillingConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder::_registrated 
  =
  aFXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder, 
      new bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder()
    )
  );
  
  bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder
    ::bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder() {
  }

  bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder
    ::~bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder() {

  }

  void bSplineSurface_constrainedFillingConstructOCCAFXmlBuilder::buildPart(
		::QDomElement const & toBuild, 
		baseContainer * const bC,
		cVPtrVec const * const cV, 
		aFPtrVec const * const aF,
		aFPtrVec * result 
	) const {
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticFunction", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("maxSeg", toBuild)
      ||
      !dtXmlParserBase::hasAttribute("maxDeg", toBuild), 
      buildPart()
    );   
    

    vectorHandling< dtCurve const * > cc;
    std::vector< ::QDomElement > elVec 
    = 
    dtXmlParserBase::getChildVector("analyticFunction", toBuild);
    dt__forAllIndex(elVec, ii) {
      dt__pH(analyticFunction) theF(
        dtXmlParserBase::createAnalyticFunction(
          &elVec[ii], bC, cV, aF
        )
      );
      dt__ptrAss(
        vec3dCurveOneD const * v3d1d,
        vec3dCurveOneD::ConstDownCast(theF.get())
      );
      cc.push_back( v3d1d->ptrDtCurve()->clone() );
    }
    dt__throwIf(cc.size()!=4, buildPart());

    ptrHandling<dtSurface> dtS( 
      bSplineSurface_constrainedFillingConstructOCC(
        cc[0], cc[1], cc[2], cc[3],
        dtXmlParserBase::getAttributeInt("maxDeg", toBuild),
        dtXmlParserBase::getAttributeInt("maxSeg", toBuild)            
      ).result()
    );

    if ( dtXmlParserBase::hasAttribute("orientation", toBuild) ) {
      dt__forAllRefAuto(
        dtXmlParserBase::getAttributeStrVector(
          "orientation", toBuild
        ), 
        aOrient
      ) {
        if (aOrient == "exchange") {
          dtS.reset( 
            bSplineSurface_exchangeSurfaceConstructOCC(dtS.get()).result() 
          );
        }
      }
    }

    //
    // create scaCurve2dOneD
    //
    result->push_back( new vec3dSurfaceTwoD( dtS.get() ) );			
  }
}