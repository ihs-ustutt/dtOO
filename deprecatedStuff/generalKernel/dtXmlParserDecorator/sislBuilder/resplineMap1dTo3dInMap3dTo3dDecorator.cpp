#include "resplineMap1dTo3dInMap3dTo3dDecorator.h"
#include "spline3dDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/map2dTo3dInMap3dTo3d.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <analyticGeometryHeaven/map3dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  resplineMap1dTo3dInMap3dTo3dDecorator::resplineMap1dTo3dInMap3dTo3dDecorator() {
  }

  resplineMap1dTo3dInMap3dTo3dDecorator::~resplineMap1dTo3dInMap3dTo3dDecorator() {
  }

  void resplineMap1dTo3dInMap3dTo3dDecorator::buildPart( 
    QDomElement ** toBuildP,
    pointContainer * const pointContainerP,
    vectorContainer * const vectorContainerP,            
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
    bool hasGeos = hasChild("map1dTo3d", **toBuildP) && hasChild("map3dTo3d", **toBuildP);
    
    if ( hasGeos ) {
      //
      // get analyticGeometry
      //
      analyticGeometry const * aG 
      = 
      depAGeoP->get( 
        getAttributeStr("label", getChild( "map1dTo3d", **toBuildP )) 
      );

      //
      // check if it is a map2dTo3d
      //
      map1dTo3d const * m1d;
      dt__mustCast(aG, map1dTo3d const, m1d);

      //
      // get analyticGeometry
      //
      aG 
      = 
      depAGeoP->get( 
        getAttributeStr("label", getChild( "map3dTo3d", **toBuildP )) 
      );

      //
      // check if it is a map3dTo3d
      //
      map3dTo3d const * m3d;
      dt__mustCast(aG, map3dTo3d const, m3d);      

      //
      // create analyticGeometry
      //      
      aGeoP->push_back( m3d->reparamInVolume(m1d) );
    }
  }
}
