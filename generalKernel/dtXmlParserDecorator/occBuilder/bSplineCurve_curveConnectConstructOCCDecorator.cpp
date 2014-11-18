#include "bSplineCurve_curveConnectConstructOCCDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_curveConnectConstructOCC.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_curveConnectConstructOCCDecorator::bSplineCurve_curveConnectConstructOCCDecorator() {
  }

  bSplineCurve_curveConnectConstructOCCDecorator::~bSplineCurve_curveConnectConstructOCCDecorator() {
  }

  void bSplineCurve_curveConnectConstructOCCDecorator::buildPart(QDomElement ** toBuildP,
                                           pointContainer * const pointContainerP,
                                           vectorContainer * const vectorContainerP,    
                                           vectorHandling< constValue * > const * const cValP,        
                                           vectorHandling< analyticFunction * > const * const sFunP,
                                           vectorHandling< analyticGeometry * > const * const depAGeoP,   
                                           vectorHandling< analyticGeometry * > * aGeoP ) const {

    /* ---------------------------------------------------------------------- */    
    /* check input */
    /* ---------------------------------------------------------------------- */    
    bool hasAGeo = hasChild("analyticGeometry", **toBuildP);

    if (hasAGeo) {
//      std::vector< dtPoint3 > workingPointP;
      QDomElement wElement = getChild("analyticGeometry", **toBuildP);
			vectorHandling< dtCurve const * > ccV;
      while ( !wElement.isNull() ) {
					analyticGeometry * aG
				  =
					this->createAnalyticGeometry( 
					  &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
					);
			  dt__PTRASS(splineCurve3d const * s3, splineCurve3d::ConstDownCast(aG));					
				ccV.push_back(s3->ptrConstDtCurve());
  			wElement = getNextSibling("analyticGeometry", wElement);
      }
			ptrHandling<dtCurve> dtC(bSplineCurve_curveConnectConstructOCC(ccV).result());
			aGeoP->push_back( new splineCurve3d(dtC.get()) );
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAGeo) );
    }
  }
}