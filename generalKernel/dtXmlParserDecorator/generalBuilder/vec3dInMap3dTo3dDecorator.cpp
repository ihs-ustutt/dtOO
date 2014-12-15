#include "vec3dInMap3dTo3dDecorator.h"
#include "functionHeaven/vec3dTransVolThreeD.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticGeometryHeaven/vec3dOneDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dTwoDInMap3dTo3d.h>
#include <analyticGeometryHeaven/vec3dThreeDInMap3dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <functionHeaven/vec3dOneD.h>
#include <functionHeaven/vec3dTwoD.h>
#include <functionHeaven/vec3dThreeD.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  vec3dInMap3dTo3dDecorator::vec3dInMap3dTo3dDecorator() {
  }

  vec3dInMap3dTo3dDecorator::~vec3dInMap3dTo3dDecorator() {
  }

  void vec3dInMap3dTo3dDecorator::buildPart( 
    QDomElement ** toBuildP,
    baseContainer * const bC,            
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
		
    bool hasAG = hasChild("analyticGeometry", **toBuildP);
		bool hasAF = hasChild("analyticFunction", **toBuildP);
    
    if ( hasAG && hasAF ) {
      //
      // get analyticGeometry
      //
			QDomElement aGElement = getChild("analyticGeometry", **toBuildP);
      dt__pH(analyticGeometry const) aG(
        createAnalyticGeometry(&aGElement, bC, cValP, sFunP, depAGeoP)
			);
        
//      );
      //
      // check if it is a map3dTo3d
      //
      dt__PTRASS( map3dTo3d const * m3d, map3dTo3d::ConstDownCast(aG.get()) );
			
      //
      // get analyticFunction
      //
			QDomElement aFElement = getChild("analyticFunction", **toBuildP);
      dt__pH(analyticFunction const) aF(
			  createAnalyticFunction(&aFElement, bC, cValP, sFunP)
			);

			vec3dOneD const * v1d = vec3dOneD::ConstDownCast(aF.get());
			vec3dTwoD const * v2d = vec3dTwoD::ConstDownCast(aF.get());
			vec3dThreeD const * v3d = vec3dThreeD::ConstDownCast(aF.get());
			
			if (v1d) {
				aGeoP->push_back( new vec3dOneDInMap3dTo3d(v1d, m3d) );
			}
			else if (v2d) {
			  aGeoP->push_back( new vec3dTwoDInMap3dTo3d(v2d, m3d) );
			}
			else if (v3d) {
			  aGeoP->push_back( new vec3dThreeDInMap3dTo3d(v3d, m3d) );
			}
			else {
				dt__THROW(
				  buildPart(), 
					<< DTLOGEVAL(v1d) << LOGDEL
					<< DTLOGEVAL(v2d) << LOGDEL
					<< DTLOGEVAL(v3d)
				);
			}
    }
  }
	
  void vec3dInMap3dTo3dDecorator::buildPartCompound( 
    QDomElement ** toBuildP,
    baseContainer * const bC,     
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
		
    bool hasAG = hasChild("analyticGeometry", **toBuildP);
		bool hasAF = hasChild("analyticFunction", **toBuildP);

    if ( hasAG && hasAF ) {
      //
      // get analyticGeometry
      //
      analyticGeometry const * aG 
      = 
      depAGeoP->get( 
        getAttributeStr("label", getChild( "analyticGeometry", **toBuildP )) 
      );
      //
      // check if it is a map3dTo3d
      //
      dt__PTRASS( 
			  map3dTo3d const * m3d, 
				map3dTo3d::ConstDownCast(aG) 
			);
			
      //
      // get analyticFunction
      //
      analyticFunction const * aF
      = 
      sFunP->get( 
        getAttributeStr("label", getChild( "analyticFunction", **toBuildP )) 
      );

//			vec3dOneD const * v1d = vec3dOneD::ConstDownCast(aF);
//			vec3dTwoD const * v2d = vec3dTwoD::ConstDownCast(aF);
			vec3dTransVolThreeDCompound const * v3d = vec3dTransVolThreeDCompound::ConstDownCast(aF);
//			
//			if (v1d) {
//				aGeoP->push_back( new vec3dOneDInMap3dTo3d(v1d, m3d) );
//			}
//			else if (v2d) {
//			  aGeoP->push_back( new vec3dTwoDInMap3dTo3d(v2d, m3d) );
//			}
			if (v3d) {
				vec3dThreeDInMap3dTo3dCompound * v3dC = new vec3dThreeDInMap3dTo3dCompound();
				for (int ii=0; ii<v3d->nComponents(); ii++) {
					v3dC->addComponent( vec3dThreeDInMap3dTo3d( &(v3d->component(ii)), m3d) );
//			    aGeoP->push_back( new vec3dThreeDInMap3dTo3d(v3d, m3d) );
				}
				aGeoP->push_back( v3dC );
			}
			else {
				dt__THROW(
				  buildPart(), 
//					<< DTLOGEVAL(v1d) << LOGDEL
//					<< DTLOGEVAL(v2d) << LOGDEL
					<< DTLOGEVAL(v3d)
				);
			}
    }
  }	
}
