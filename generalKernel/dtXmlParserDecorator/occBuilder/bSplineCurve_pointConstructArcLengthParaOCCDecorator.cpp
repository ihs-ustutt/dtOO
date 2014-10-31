#include "bSplineCurve_pointConstructArcLengthParaOCCDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCBezierCurve.h>
#include <geometryEngine/geoBuilder/bSplineCurve_pointConstructArcLengthParaOCC.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  bSplineCurve_pointConstructArcLengthParaOCCDecorator::bSplineCurve_pointConstructArcLengthParaOCCDecorator() {
  }

  bSplineCurve_pointConstructArcLengthParaOCCDecorator::~bSplineCurve_pointConstructArcLengthParaOCCDecorator() {
  }

  void bSplineCurve_pointConstructArcLengthParaOCCDecorator::buildPart(QDomElement ** toBuildP,
                                           pointContainer * const pointContainerP,
                                           vectorContainer * const vectorContainerP,    
                                           vectorHandling< constValue * > const * const cValP,        
                                           vectorHandling< analyticFunction * > const * const sFunP,
                                           vectorHandling< analyticGeometry * > const * const depAGeoP,   
                                           vectorHandling< analyticGeometry * > * aGeoP ) const {
    bool hasAG = hasChild("analyticGeometry", **toBuildP);
		bool hasTol = hasAttribute("tolerance", **toBuildP);
		bool hasMaxOrder = hasAttribute("max_order", **toBuildP);
		bool hasSegments  = hasAttribute("num_segments", **toBuildP);

    if (hasAG && hasTol && hasMaxOrder && hasSegments ) {
			int const maxOrder = getAttributeIntMuParse("max_order", **toBuildP, cValP, sFunP);
			int const nSegments = getAttributeIntMuParse("num_segments", **toBuildP, cValP, sFunP);
			float const tol = getAttributeFloatMuParse("tolerance", **toBuildP, cValP, sFunP);
			
      QDomElement wElement = getChild("analyticGeometry", **toBuildP);
      analyticGeometry * aG 
			=
			createAnalyticGeometry(
  			&wElement, 
        pointContainerP, vectorContainerP, 
        cValP, sFunP, depAGeoP
		  );
			dt__PTRASS(splineCurve3d const * sC3, splineCurve3d::ConstDownCast(aG));
  		aGeoP->push_back( 
			  new splineCurve3d(
			    bSplineCurve_pointConstructArcLengthParaOCC(
			      sC3->ptrConstDtCurve(), tol, maxOrder, nSegments
			    ).result()
			  )
			);
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAG) << LOGDEL
							<< DTLOGEVAL(hasTol) << LOGDEL
							<< DTLOGEVAL(hasMaxOrder) << LOGDEL
							<< DTLOGEVAL(hasSegments) );
    }
  }
}