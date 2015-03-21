#include "surfaceOfRevolution_curveRotateConstructOCCDecorator.h"
#include "geometryEngine/geoBuilder/trimmedCurve_twoPointsConnectConstructOCC.h"
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <geometryEngine/dtCurve.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtOCCSurface.h>
#include <geometryEngine/geoBuilder/surfaceOfRevolution_curveRotateConstructOCC.h>
#include <geometryEngine/geoBuilder/rectangularTrimmedSurface_curveRotateConstructOCC.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  surfaceOfRevolution_curveRotateConstructOCCDecorator::surfaceOfRevolution_curveRotateConstructOCCDecorator() {
  }

  surfaceOfRevolution_curveRotateConstructOCCDecorator::~surfaceOfRevolution_curveRotateConstructOCCDecorator() {
  }

  void surfaceOfRevolution_curveRotateConstructOCCDecorator::buildPart(
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
		bool hasOrigin = hasChild("Point_3", **toBuildP);
		bool hasRotAxis = hasChild("Vector_3", **toBuildP);
		
    if (hasAG && hasOrigin && hasRotAxis) {
			QDomElement wElement;
			//
			// spline curve
			//
			wElement = getChild("analyticGeometry", **toBuildP);
			analyticGeometry * aG
			=
			this->createAnalyticGeometry(&wElement, bC, cValP, sFunP, depAGeoP);
			dt__ptrAss(splineCurve3d * s3, splineCurve3d::DownCast(aG) );
			dtCurve const * cc = s3->ptrConstDtCurve();
			//
			// vector
			//
			wElement = getChild("Vector_3", **toBuildP);
      dtVector3 vv
			=
			this->createDtVector3(&wElement, bC, cValP, sFunP, depAGeoP);
			//
			// point
			//
			wElement = getChild("Point_3", **toBuildP);
      dtPoint3 pp 
			= 
			this->createDtPoint3(&wElement, bC, cValP, sFunP, depAGeoP);
			//
			// angle
			//
			bool hasAngle = hasChild("float", **toBuildP);
			float angle = 2.*M_PI;
			if ( hasAngle ) {
				angle
				=
				getAttributeFloatMuParse(
			    "value", 
					getChild("float", **toBuildP),
					cValP, 
					sFunP
				);
			}
			ptrHandling<dtSurface> dtS;
			
			if (!hasAngle) {
				dtS.reset(
					surfaceOfRevolution_curveRotateConstructOCC(*cc, pp, vv).result()
				);
			}
			else {
				dtS.reset(
					rectangularTrimmedSurface_curveRotateConstructOCC(*cc, pp, vv, angle).result()
				);
			}
			rotatingSpline * rS = new rotatingSpline(*dtS, pp, vv, angle);
			
			rS->correctOrigin();
			
			aGeoP->push_back(rS);
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAG) << LOGDEL
							<< DTLOGEVAL(hasOrigin) << LOGDEL
							<< DTLOGEVAL(hasRotAxis) );
    }
  }
}