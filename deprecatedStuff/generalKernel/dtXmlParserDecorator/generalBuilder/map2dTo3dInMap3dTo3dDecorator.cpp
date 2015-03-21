#include "map2dTo3dInMap3dTo3dDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/map2dTo3dInMap3dTo3d.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  map2dTo3dInMap3dTo3dDecorator::map2dTo3dInMap3dTo3dDecorator() {
  }

  map2dTo3dInMap3dTo3dDecorator::~map2dTo3dInMap3dTo3dDecorator() {
  }

  void map2dTo3dInMap3dTo3dDecorator::buildPart( 
    QDomElement ** toBuildP,
    pointContainer * const pointContainerP,
    vectorContainer * const vectorContainerP,            
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
    bool hasGeos = hasChild("map2dTo3d", **toBuildP) && hasChild("map3dTo3d", **toBuildP);
    
    if ( hasGeos ) {
      QDomElement wElement = getChild( "map2dTo3d", **toBuildP );
      
      //
      // get analyticGeometry
      //
      analyticGeometry const * aG 
      = 
      depAGeoP->get( 
        getAttributeStr("label", getChild( "map2dTo3d", **toBuildP )) 
      );

      //
      // check if it is a map2dTo3d
      //
      map2dTo3d const * m2d;
      dt__mustDownCast(aG, map2dTo3d const, m2d);

      //
      // get analyticGeometry
      //
      aG 
      = 
      depAGeoP->get( 
        getAttributeStr("label", getChild( "map3dTo3d", **toBuildP )) 
      );

      //
      // check if it is a map2dTo3d
      //
      map3dTo3d const * m3d;
      dt__mustDownCast(aG, map3dTo3d const, m3d);      

      //
      // create analyticGeometry
      //      
      aGeoP->push_back( new map2dTo3dInMap3dTo3d(m2d, m3d) );
    }
  }
}
