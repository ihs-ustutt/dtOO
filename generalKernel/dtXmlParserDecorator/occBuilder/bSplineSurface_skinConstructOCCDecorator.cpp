#include "bSplineSurface_skinConstructOCCDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/bSplineSurface_skinConstructOCC.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineSurface_skinConstructOCCDecorator::bSplineSurface_skinConstructOCCDecorator() {
  }

  bSplineSurface_skinConstructOCCDecorator::~bSplineSurface_skinConstructOCCDecorator() {
  }

  void bSplineSurface_skinConstructOCCDecorator::buildPart(
	  QDomElement ** toBuildP,
    baseContainer * const bC, 
		vectorHandling< constValue * > const * const cValP,        
		vectorHandling< analyticFunction * > const * const sFunP,
		vectorHandling< analyticGeometry * > const * const depAGeoP,   
		vectorHandling< analyticGeometry * > * aGeoP 
	) const {

    /* ---------------------------------------------------------------------- */    
    /* check input */
    /* ---------------------------------------------------------------------- */    
    bool hasAG = hasChild("analyticGeometry", **toBuildP);

    if (hasAG) {	
			int aGOff = 1;
			if ( hasAttribute("analyticGeometry_offset", **toBuildP) ) {
				aGOff 
				= 
				getAttributeIntMuParse(
				  "analyticGeometry_offset", **toBuildP, cValP, sFunP
				);
			}
      vectorHandling< dtCurve const * > ccV;
      std::vector<QDomElement> wEl = getChildVector("analyticGeometry", **toBuildP);
			int ii = 0;
			vectorHandling< splineCurve3d const * > s3Vec(aGOff);
		  vectorHandling< dtCurve const * > dtCVec(aGOff);
      while ( ii < wEl.size() ) {			
				for (int jj=0;jj<aGOff;jj++) {
					analyticGeometry * aG
					=
					this->createAnalyticGeometry(&(wEl[ii]), bC, cValP, sFunP, depAGeoP);
					dt__PTRASS(s3Vec[jj], splineCurve3d::ConstDownCast(aG));
					dtCVec[jj] = s3Vec[jj]->ptrConstDtCurve();
					ii++;
			  }
				splineCurve3d const * s3;
				if (aGOff == 1) {
				  dt__PTRASS(s3, splineCurve3d::ConstDownCast( s3Vec[0]) );
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
			aGeoP->push_back( 
			  new analyticSurface(
			    dt__tmpPtr(
		        dtSurface, 
			      bSplineSurface_skinConstructOCC(ccV).result()
			    )
			  )
			);
			//
			// free memory
			//
//			ccV.destroy();
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAG) );
    }
  }
}