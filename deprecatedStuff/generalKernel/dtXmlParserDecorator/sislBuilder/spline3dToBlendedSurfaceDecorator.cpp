#include "spline3dToBlendedSurfaceDecorator.h"
#include "analyticGeometryHeaven/splineSurface3d.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineSurface3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  spline3dToBlendedSurfaceDecorator::spline3dToBlendedSurfaceDecorator() {
  }

  spline3dToBlendedSurfaceDecorator::~spline3dToBlendedSurfaceDecorator() {
  }

  void spline3dToBlendedSurfaceDecorator::buildPart(
         ::QDomElement ** toBuildP,
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
    bool hasSplineCurves3d = hasChild("splineCurve3d", **toBuildP);
    bool hasSplineSurface = hasChild("splineSurface", **toBuildP);

    if (hasSplineCurves3d && !hasSplineSurface) {
      //
      // put splines in builder
      //
			vectorHandling< analyticGeometry const * > cc;
      ::QDomElement wElement = getChild( "splineCurve3d", **toBuildP );
      while ( !wElement.isNull() ) {
        analyticGeometry * aGeoSplineP
				=
        createAnalyticGeometry(
          &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
        );
				cc.push_back( aGeoSplineP );
        //goto next sibling
        wElement = getNextSibling("splineCurve3d", wElement);
      }

      /* -------------------------------------------------------------------- */
      /* call builder and construct spline */
      /* -------------------------------------------------------------------- */
			splineSurface3d * sS = new splineSurface3d();
			sS->setConstruction("blended");
			sS->createMe(&cc);
			aGeoP->push_back(sS);
    }
    //
    // copy existing surface
    //
    else if (hasSplineSurface && !hasSplineCurves3d) {
      ::QDomElement wElement = getChild( "splineSurface", **toBuildP );
      createAdvanced(
        &wElement,
        pointContainerP,
        vectorContainerP,
        cValP,
        sFunP,
        depAGeoP,
        aGeoP
      );
    }
    else {
      dt__THROW(buildPart(),
              << dt__eval(hasSplineCurves3d) << std::endl
              << dt__eval(hasSplineSurface) );
    }
  }
}