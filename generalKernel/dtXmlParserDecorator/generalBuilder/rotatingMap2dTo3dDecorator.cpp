#include "rotatingMap2dTo3dDecorator.h"


#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3d.h>
#include <analyticGeometryHeaven/rotatingMap2dTo3dPhiMS.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/pointContainer.h>
#include <baseContainerHeaven/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  rotatingMap2dTo3dDecorator::rotatingMap2dTo3dDecorator() {
  }

  rotatingMap2dTo3dDecorator::~rotatingMap2dTo3dDecorator() {
  }

  void rotatingMap2dTo3dDecorator::buildPart( 
    QDomElement ** toBuildP,
    pointContainer * const pointContainerP,
    vectorContainer * const vectorContainerP,            
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
    bool hasGeo = hasChild("analyticGeometry", **toBuildP);
		bool hasVec = hasChild("Vector_3", **toBuildP);
    
    if ( hasGeo && hasVec ) {
			QDomElement wElement;			
      //
      // get analyticGeometry and cast
      //
			wElement = getChild("analyticGeometry", **toBuildP);
			analyticGeometry * aG
			=
			this->createAnalyticGeometry( 
			  &wElement, pointContainerP, vectorContainerP, cValP, sFunP, depAGeoP
		  );
      dt__PTRASS(map2dTo3d const * m2d, map2dTo3d::ConstDownCast(aG));
			
			//
			// get vector
			//
			wElement = getChild("Vector_3", **toBuildP);
      dtVector3 vv
			=				
			this->createDtVector3(
			  &wElement, 
				pointContainerP, 
				vectorContainerP, 
				cValP, 
				sFunP, 
			  depAGeoP
			);

      //
      // create analyticGeometry
      //
      if ( hasAttribute("phiMS", **toBuildP) ) {
			  aGeoP->push_back( new rotatingMap2dTo3dPhiMS(vv, m2d) );
				if (hasAttribute("angle", **toBuildP)) {
					float angle = getAttributeFloatMuParse("angle", **toBuildP, cValP, sFunP);
					rotatingMap2dTo3dPhiMS::SecureCast(aGeoP->back())->setAngle(angle);
				}
			}
			else {     
        aGeoP->push_back( new rotatingMap2dTo3d(vv, m2d) );
			}			
    }
  }
}
