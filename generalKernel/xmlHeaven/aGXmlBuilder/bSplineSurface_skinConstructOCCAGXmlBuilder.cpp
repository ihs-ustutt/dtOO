#include "bSplineSurface_skinConstructOCCAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineSurface_skinConstructOCCAGXmlBuilder
    ::bSplineSurface_skinConstructOCCAGXmlBuilder() {
  }

  bSplineSurface_skinConstructOCCAGXmlBuilder
    ::~bSplineSurface_skinConstructOCCAGXmlBuilder() {
  }

  void bSplineSurface_skinConstructOCCAGXmlBuilder::buildPart(
    QDomElement const & toBuild,
    baseContainer * const bC,           
    vectorHandling< constValue * > const * const cV,           
    vectorHandling< analyticFunction * > const * const aF,    
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result 
	) const {  
    //
		// check input
		//    
    dt__throwIf(!dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart());

    int aGOff = 1;
    if ( dtXmlParserBase::hasAttribute("analyticGeometry_offset", toBuild) ) {
      aGOff 
      = 
      dtXmlParserBase::getAttributeIntMuParse("analyticGeometry_offset", toBuild, cV, aF);
    }
    vectorHandling< dtCurve const * > ccV;
    std::vector< QDomElement > wEl 
    = 
    dtXmlParserBase::getChildVector("analyticGeometry", toBuild);
    int ii = 0;
    vectorHandling< splineCurve3d const * > s3Vec(aGOff);
    vectorHandling< dtCurve const * > dtCVec(aGOff);
    while ( ii < wEl.size() ) {			
      for (int jj=0;jj<aGOff;jj++) {
        analyticGeometry * aG_t
        =
        dtXmlParserBase::createAnalyticGeometry(&(wEl[ii]), bC, cV, aF, aG);
        dt__ptrAss(s3Vec[jj], splineCurve3d::ConstDownCast(aG_t));
        dtCVec[jj] = s3Vec[jj]->ptrConstDtCurve();
        ii++;
      }
      splineCurve3d const * s3;
      if (aGOff == 1) {
        dt__ptrAss(s3, splineCurve3d::ConstDownCast( s3Vec[0]) );
        ccV.push_back( s3->ptrConstDtCurve()->clone() );
      }
      else {
        ccV.push_back(
          dt__tmpPtr(
            dtCurve, 
            bSplineCurve_curveConnectConstructOCC(dtCVec).result()
          )
        );
      }
    }
    result->push_back( 
      new analyticSurface(
        dt__tmpPtr(
          dtSurface, 
          bSplineSurface_skinConstructOCC(ccV).result()
        )
      )
    );
  }
}