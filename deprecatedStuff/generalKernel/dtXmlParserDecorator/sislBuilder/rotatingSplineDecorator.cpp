#include "rotatingSplineDecorator.h"
#include "spline3dDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/rotatingSpline.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <geometryEngine/dtCurve.h>
#include <geometryEngine/dtSurface.h>
#include <geometryEngine/geoBuilder/surfaceRotateCurveConstruct.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  rotatingSplineDecorator::rotatingSplineDecorator() {
  }

  rotatingSplineDecorator::~rotatingSplineDecorator() {
  }

  /**
   * @todo clean up routine
   */
  void rotatingSplineDecorator::buildPart( 
               QDomElement ** toBuildP,
               pointContainer * const pointContainerP,
               vectorContainer * const vectorContainerP,            
               vectorHandling< constValue * > const * const cValP,           
               vectorHandling< analyticFunction * > const * const sFunP,    
               vectorHandling< analyticGeometry * > const * const depAGeoP,
               vectorHandling< analyticGeometry * > * aGeoP ) const {

    bool hasVector = hasChild("Vector_3", **toBuildP);
    bool hasPoint = hasChild("Point_3", **toBuildP);
    bool hasAGeo = hasChild("splineCurve3d", **toBuildP);
    bool hasAngle = hasChild("float", **toBuildP);

    if ( hasAGeo && hasPoint && hasVector ) {
      QDomElement wElement;
      
      //
      // origin
      //
      wElement = getChild("Point_3", **toBuildP);
      if ( wElement.hasAttribute("attribute") ) {
        if ( getAttributeStr("attribute", wElement) != "origin" ) {
          dt__warning(buildPart(),
                  << "Attribute of dtPoint3 is not origin" );
        }        
      }
      dtPoint3 originP
			=
      this->createDtPoint3( &wElement, 
                         pointContainerP, 
                         vectorContainerP, 
                         cValP, 
                         sFunP,
                         depAGeoP);

      //
      // rotation vector
      //
      wElement = getChild("Vector_3", **toBuildP);
      if ( wElement.hasAttribute("attribute") ) {
        if ( getAttributeStr("attribute", wElement) != "rotation_axis" ) {
          dt__warning(buildPart(),
                  << "Attribute of dtVector3 is not rotation_axis" );
        }
      }
      dtVector3 rotationAxisP
			=
      this->createDtVector3( 
			  &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
			);      

      //
      // splineToRotate
      //
      wElement = getChild("splineCurve3d", **toBuildP);     
      analyticGeometry * aG
			=
      this->createAnalyticGeometry( 
			  &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
			);
			dt__ptrAss(splineCurve3d * s3, splineCurve3d::DownCast(aG));
			
      //
      // angle
      //
			float angle = 2*M_PI;
      if (hasAngle) {
        wElement = getChild("float", **toBuildP);
        if ( getAttributeStr("attribute", wElement) != "angle" ) {
          dt__warning(buildPart(),
                  << "Attribute of float is not angle" );
        }
        angle = muParseString(
                  replaceUsedFunctions(
                    getAttributeStr("value", wElement),
                    cValP,
                    sFunP
                  )
                );
      }

			ptrHandling<dtSurface> dtS(
				surfaceRotateCurveConstruct(
					originP, 
					rotationAxisP, 
					*(s3->getConstDtSislCurve()),
					angle
				).result()
			);
			aGeoP->push_back(
			  new rotatingSpline(*dtS, originP, rotationAxisP, angle)
			);
    }
  }
}
