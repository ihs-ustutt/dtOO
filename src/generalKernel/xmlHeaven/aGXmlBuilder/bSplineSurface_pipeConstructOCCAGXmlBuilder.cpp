#include "bSplineSurface_pipeConstructOCCAGXmlBuilder.h"

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
          dtXmlParserBase::getAttributeFloatMuParse("radius", toBuild, cV, aF)
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
          analyticCurve::MustDownCast(aSecOne.get())->ptrConstDtCurve()
        ).result()        
      );
    }    
    else dt__throwUnexpected(buildPart());
    if (
      dtXmlParserBase::hasAttribute("exchange", toBuild)  
      && 
      dtXmlParserBase::getAttributeBool("exchange", toBuild)
    ) {
      dtS.reset( 
        bSplineSurface_exchangeSurfaceConstructOCC(dtS.get()).result()
      );
    }
    result->push_back( new analyticSurface( dtS.get() ) );    
  }
}