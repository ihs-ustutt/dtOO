#include "bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtSurface.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineSurface_geomCurveFillConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

#include <boost/assign.hpp>

namespace dtOO {
  bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder
    ::bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder() {
  }

  bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder
    ::~bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder() {
  }

  std::vector< std::string > 
    bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder::factoryAlias( 
      void 
  ) const {
    return 
      ::boost::assign::list_of("bSplineSurface_bSplineCurveFillConstructOCC");
  }
  
  void bSplineSurface_geomCurveFillConstructOCCAGXmlBuilder::buildPart(
    ::QDomElement const & toBuild,
    baseContainer * const bC,           
    cVPtrVec const * const cV,           
    aFPtrVec const * const aF,    
    aGPtrVec const * const aG,
    aGPtrVec * result 
	) const {
    //
		// check input
		//    
    dt__throwIf(!dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart());
		
    aGPtrVec aG_t;
    ::QDomElement wElement = dtXmlParserBase::getChild("analyticGeometry", toBuild);
    while ( !wElement.isNull() ) {
      dtXmlParserBase::createAdvanced( &wElement, bC, cV, aF, aG, &aG_t );
      wElement = dtXmlParserBase::getNextSibling("analyticGeometry", wElement);
    }

    vectorHandling< dtCurve const * > dtC( aG_t.size() );
    dt__forAllIndex(aG_t, ii) {
      dt__ptrAss(
        splineCurve3d const * s3, splineCurve3d::ConstDownCast(aG_t[ii])
      );
      dtC[ii] = s3->ptrConstDtCurve();
    }

    ptrHandling<dtSurface> dtS(
      bSplineSurface_geomCurveFillConstructOCC(dtC).result()
    );
    result->push_back( new analyticSurface(dtS.get()) );
  }
}