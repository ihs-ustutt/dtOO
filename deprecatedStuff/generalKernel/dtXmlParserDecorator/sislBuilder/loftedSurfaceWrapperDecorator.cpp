#include "loftedSurfaceWrapperDecorator.h"
#include <progHelper.h>
#include <logMe/logMe.h>

#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <dtXmlParserDecorator/dtXmlParserDecoratorInterface.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <analyticGeometryHeaven/splineSurface3d.h>
#include <analyticGeometryHeaven/oneDimCloseable.h>

namespace dtOO { 
  loftedSurfaceWrapperDecorator::loftedSurfaceWrapperDecorator() {
  }

  loftedSurfaceWrapperDecorator::~loftedSurfaceWrapperDecorator() {
  }  

  void loftedSurfaceWrapperDecorator::buildPart(
         QDomElement ** toBuildP,
         pointContainer * const pointContainerP,
         vectorContainer * const vectorContainerP,            
         vectorHandling< constValue * > const * const cValP,           
         vectorHandling< analyticFunction * > const * const sFunP,    
         vectorHandling< analyticGeometry * > const * const depAGeoP,
         vectorHandling< analyticGeometry * > * aGeoP 
  ) const {

    bool hasAGeo = hasChild("analyticGeometry", **toBuildP);
    bool hasAGeoOffset = hasAttribute("analyticGeometry_offset", **toBuildP);
    bool hasOrder = hasAttribute("order", **toBuildP);
    
    //
    // initialize builder
    //
    vectorHandling< analyticGeometry const * > s3Vec;
    
    if ( hasAGeo && hasOrder && hasAGeoOffset) {
      //
      // get input
      //
      int aGeoOffset = getAttributeInt("analyticGeometry_offset", **toBuildP);
      std::string closing = getAttributeStr("closing", **toBuildP);
      //
      // initialize builder
      //
    
      //
      // analyticGeometries
      //
      QDomElement wElement = getChild("analyticGeometry", **toBuildP);     
      analyticGeometry * tmpGeo;
      vectorHandling< analyticGeometry * > cc;
      while ( !wElement.isNull() ) {
        for (int ii=0;ii<aGeoOffset;ii++) {
					tmpGeo
					=
          this->createAnalyticGeometry( 
					  &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
					);
          cc.push_back( tmpGeo );
          wElement = getNextSibling("analyticGeometry", wElement);
        }
        analyticGeometry * sC = new splineCurve3d(cc);
				cc.destroy();
        //
        // close splineCurve3d
        //
        if (closing == "closeArithmetic") {
          oneDimCloseable * oDC;
          dt__mustCastWM(
            sC, 
            oneDimCloseable, 
            oDC, 
            << "analyticGeometry must be closeable."
          );
          oDC->closeArithmetic();
        }
        else if (closing == "closeStraight") {
          oneDimCloseable * oDC;
          dt__mustCastWM(
            sC, 
            oneDimCloseable, 
            oDC, 
            << "analyticGeometry must be closeable."
          );
          oDC->closeStraight();
        }

        //
        // put splineCurve3d to vector
        //
        s3Vec.push_back( sC );
      }
    }
    else {
      dt__THROW(buildPart(),
              << dt__eval(hasAGeo) << std::endl
              << dt__eval(hasOrder) << std::endl
              << dt__eval(hasAGeoOffset) );      
    }
    
    //
    // call builder
    //
    splineSurface3d sS3d;
    sS3d.setLoftDirectionOrder( getAttributeInt("order", **toBuildP) );
    sS3d.createMe( &s3Vec );
    
    aGeoP->push_back( sS3d.clone() ); 
    s3Vec.destroy();
  }
}

