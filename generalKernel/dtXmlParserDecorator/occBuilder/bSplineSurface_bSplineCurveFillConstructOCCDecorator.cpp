#include "bSplineSurface_bSplineCurveFillConstructOCCDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtSurface.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticGeometryHeaven/splineSurface3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineSurface_bSplineCurveFillConstructOCC.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineSurface_bSplineCurveFillConstructOCCDecorator::bSplineSurface_bSplineCurveFillConstructOCCDecorator() {
  }

  bSplineSurface_bSplineCurveFillConstructOCCDecorator::~bSplineSurface_bSplineCurveFillConstructOCCDecorator() {
  }

  void bSplineSurface_bSplineCurveFillConstructOCCDecorator::buildPart(
	  QDomElement ** toBuildP,
		pointContainer * const pointContainerP,
		vectorContainer * const vectorContainerP,    
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
      vectorHandling< analyticGeometry * > aG;
      QDomElement wElement = getChild("analyticGeometry", **toBuildP);
      while ( !wElement.isNull() ) {
        this->createAdvanced( &wElement, 
                           pointContainerP, 
                           vectorContainerP, 
                           cValP, 
                           sFunP, 
                           depAGeoP, 
                           &aG );
  			wElement = getNextSibling("analyticGeometry", wElement);
      }
			
			vectorHandling< dtCurve const * > dtC( aG.size() );
			dt__FORALL(aG, ii,
			  dt__PTRASS(splineCurve3d const * s3, splineCurve3d::ConstDownCast(aG[ii]));
			  dtC[ii] = s3->ptrConstDtCurve();
			);
			
			ptrHandling<dtSurface> dtS(
			  bSplineSurface_bSplineCurveFillConstructOCC(dtC).result()
			);
			aGeoP->push_back( new splineSurface3d(dtS.get()) );
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAG) );
    }
  }
}