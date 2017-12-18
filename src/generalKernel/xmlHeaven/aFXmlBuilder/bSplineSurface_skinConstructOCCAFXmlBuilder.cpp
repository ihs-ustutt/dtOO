#include "bSplineSurface_skinConstructOCCAFXmlBuilder.h"

#include <xmlHeaven/dtXmlParserBase.h>
#include "geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h"
#include <analyticFunctionHeaven/analyticFunction.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <analyticFunctionHeaven/vec3dCurveOneD.h>
#include <analyticFunctionHeaven/vec3dSurfaceTwoD.h>
#include <logMe/logMe.h>
#include <interfaceHeaven/ptrHandling.h>
#include <baseContainerHeaven/baseContainer.h>

#include <boost/assign.hpp>
#include <QtXml/QDomElement>

namespace dtOO {
  bSplineSurface_skinConstructOCCAFXmlBuilder
    ::bSplineSurface_skinConstructOCCAFXmlBuilder() {
  }

  bSplineSurface_skinConstructOCCAFXmlBuilder
    ::~bSplineSurface_skinConstructOCCAFXmlBuilder() {

  }

  std::vector< std::string > bSplineSurface_skinConstructOCCAFXmlBuilder
    ::factoryAlias( void ) const {
    return ::boost::assign::list_of
      ("vec3dSurfaceTwoD")
      ("vec3dSurfaceTwoDAFXmlBuilder");
  }
  
  void bSplineSurface_skinConstructOCCAFXmlBuilder::buildPart(
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

    //
    // set input
    //
    vectorHandling< dtCurve const * > cL;    
    dt__forAllRefAuto(
      dtXmlParserBase::getChildVector("analyticFunction", toBuild), anEl
    ) {
      aFPtrVec aFPtrV;
      dtXmlParserBase::createAdvanced(anEl, bC, cV, aF, &aFPtrV);
      dt__forAllRefAuto(aFPtrV, aF_t) {
        dt__ptrAss(
          vec3dCurveOneD const * v3d1d, vec3dCurveOneD::ConstDownCast(aF_t)
        );
        cL.push_back( v3d1d->ptrConstDtCurve()->clone() );
      }
      aFPtrV.destroy();
    }
    
    int nIter = 0;
    if ( dtXmlParserBase::hasAttribute("nIterations", toBuild) ) {
      nIter 
      = 
      dtXmlParserBase::getAttributeIntMuParse("nIterations", toBuild, cV, aF);
    }
    int minDeg = 1;
    if ( dtXmlParserBase::hasAttribute("orderMin", toBuild) ) {
      minDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMin", toBuild, cV, aF);
    }    
    int maxDeg = 25;
    if ( dtXmlParserBase::hasAttribute("orderMax", toBuild) ) {
      maxDeg 
      = 
      dtXmlParserBase::getAttributeIntMuParse("orderMax", toBuild, cV, aF);
    }
    
    dt__pH(dtSurface) dtS( 
      bSplineSurface_skinConstructOCC(cL, minDeg, maxDeg, nIter).result() 
    );

    //
    // create vec3dSurfaceTwoD
    //
    result->push_back( new vec3dSurfaceTwoD( dtS.get() ) );			
  }
}
