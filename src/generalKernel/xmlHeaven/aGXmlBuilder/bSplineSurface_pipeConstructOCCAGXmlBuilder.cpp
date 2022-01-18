#include "bSplineSurface_pipeConstructOCCAGXmlBuilder.h"

#include <xmlHeaven/aGXmlBuilderFactory.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/analyticCurve.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_pipeConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineSurface_exchangeSurfaceConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bool bSplineSurface_pipeConstructOCCAGXmlBuilder::_registrated 
  =
  aGXmlBuilderFactory::registrate(
    dt__tmpPtr(
      bSplineSurface_pipeConstructOCCAGXmlBuilder, 
      new bSplineSurface_pipeConstructOCCAGXmlBuilder()
    )
  );
  
  bSplineSurface_pipeConstructOCCAGXmlBuilder
    ::bSplineSurface_pipeConstructOCCAGXmlBuilder() {
  }

  bSplineSurface_pipeConstructOCCAGXmlBuilder
    ::~bSplineSurface_pipeConstructOCCAGXmlBuilder() {
  }

  void bSplineSurface_pipeConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {  
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild),
      buildPart()
    );

    bool polynomial 
    = 
    dtXmlParserBase::getAttributeBool("polynomial", toBuild, false);
    dtInt absShape 
    = 
    dtXmlParserBase::getAttributeIntMuParse("absShape", toBuild, cV, aF, 2);
    dtInt maxDegree 
    = 
    dtXmlParserBase::getAttributeIntMuParse("maxDegree", toBuild, cV, aF, 11);
    dtInt nbSections 
    = 
    dtXmlParserBase::getAttributeIntMuParse("nbSections", toBuild, cV, aF, 30);
    
    std::vector< ::QDomElement > wE_v 
    = 
    dtXmlParserBase::getChildVector("analyticGeometry", toBuild);
    
    dt__pH(dtSurface) dtS;
    if ( 
      (wE_v.size()==1) && dtXmlParserBase::hasAttribute("radius", toBuild)
    ) {
      dt__pH(analyticGeometry) aPath(
        dtXmlParserBase::createAnalyticGeometry(wE_v[0], bC, cV, aF, aG)
      );
      dtS.reset(
        bSplineSurface_pipeConstructOCC(
          analyticCurve::MustDownCast(aPath.get())->ptrConstDtCurve(),
          dtXmlParserBase::getAttributeFloatMuParse("radius", toBuild, cV, aF),
          polynomial, 
          absShape, 
          maxDegree, 
          nbSections               
        ).result()
      );
    }
    else if ( wE_v.size()==2 ) {
      dt__pH(analyticGeometry) aPath(
        dtXmlParserBase::createAnalyticGeometry(wE_v[0], bC, cV, aF, aG)
      );
      dt__pH(analyticGeometry) aSecOne(
        dtXmlParserBase::createAnalyticGeometry(wE_v[1], bC, cV, aF, aG)
      );      
      dtS.reset(      
        bSplineSurface_pipeConstructOCC(
          analyticCurve::MustDownCast(aPath.get())->ptrConstDtCurve(),
          analyticCurve::MustDownCast(aSecOne.get())->ptrConstDtCurve(),
          polynomial, 
          absShape, 
          maxDegree, 
          nbSections
        ).result()        
      );
    }    
    else if ( wE_v.size()==3 ) {
      dt__pH(analyticGeometry) aPath(
        dtXmlParserBase::createAnalyticGeometry(wE_v[0], bC, cV, aF, aG)
      );
      dt__pH(analyticGeometry) aSecOne(
        dtXmlParserBase::createAnalyticGeometry(wE_v[1], bC, cV, aF, aG)
      );      
      dt__pH(analyticGeometry) aSecTwo(
        dtXmlParserBase::createAnalyticGeometry(wE_v[2], bC, cV, aF, aG)
      );      
      dtS.reset(
        bSplineSurface_pipeConstructOCC(
          analyticCurve::MustDownCast(aPath.get())->ptrConstDtCurve(),
          analyticCurve::MustDownCast(aSecOne.get())->ptrConstDtCurve(),
          analyticCurve::MustDownCast(aSecTwo.get())->ptrConstDtCurve(),
          polynomial, 
          absShape, 
          maxDegree, 
          nbSections              
        ).result()        
      );
    }        
    else dt__throwUnexpected(buildPart());
    
    if ( dtXmlParserBase::getAttributeBool("exchange", toBuild, false) ) {
      dtS.reset( 
        bSplineSurface_exchangeSurfaceConstructOCC(dtS.get()).result()
      );
    }
    result->push_back( new analyticSurface( dtS.get() ) );    
  }
}