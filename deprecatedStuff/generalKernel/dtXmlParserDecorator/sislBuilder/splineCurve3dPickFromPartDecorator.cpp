#include "splineCurve3dPickFromPartDecorator.h"
#include <progHelper.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <logMe/logMe.h>

#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/splineCurve3d.h>
#include <dtXmlParserDecorator/dtXmlParserDecoratorInterface.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainer/pointContainer.h>
#include <baseContainer/vectorContainer.h>

namespace dtOO { 
  splineCurve3dPickFromPartDecorator::splineCurve3dPickFromPartDecorator() {
  }

  splineCurve3dPickFromPartDecorator::~splineCurve3dPickFromPartDecorator() {
  }  

  void splineCurve3dPickFromPartDecorator::buildPart(
         QDomElement ** toBuildP,
         pointContainer * const pointContainerP,
         vectorContainer * const vectorContainerP,            
         vectorHandling< constValue * > const * const cValP,           
         vectorHandling< analyticFunction * > const * const sFunP,    
         vectorHandling< analyticGeometry * > const * const depAGeoP,
         vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
    bool hasAGeo = hasChild("Point_2", **toBuildP);
    bool hasPartLabel = hasAttribute("part_label", **toBuildP);
    bool hasOrder = hasAttribute("order", **toBuildP);
    
    if ( hasAGeo && hasOrder && hasPartLabel ) {
      int order = getAttributeInt("order", **toBuildP);         

      //
      // get mapping
      //
      analyticGeometry * aG 
      = 
      depAGeoP->get(
        getAttributeStr(
          "part_label", 
          **toBuildP
        )
      );
      
      map2dTo3d * map;
      dt__mustDownCast(aG, map2dTo3d, map);
    
      //
      // analyticGeometries
      //
      QDomElement wElement = getChild("Point_2", **toBuildP);     
      vectorHandling< dtPoint2 * > tmpPoint;
			vectorHandling< dtPoint3 > p3Point;
      while ( !wElement.isNull() ) {
          this->createBasic( &wElement, 
                             pointContainerP, 
                             vectorContainerP, 
                             cValP, 
                             sFunP,
                             depAGeoP,
                             &tmpPoint );
          for (int ii=0;ii<tmpPoint.size();ii++) {
            p3Point.push_back(
						  map->getPoint( tmpPoint[ii]->x(), tmpPoint[ii]->y() )
						);
          }
					tmpPoint.destroy();
          wElement = getNextSibling("Point_2", wElement);
      }

      //
      // create
			//
			aGeoP->push_back( new splineCurve3d(&p3Point, order) );
    }
    else {
      dt__THROW(buildPart(),
              << DTLOGEVAL(hasAGeo) << LOGDEL
              << DTLOGEVAL(hasOrder) << LOGDEL
              << DTLOGEVAL(hasPartLabel) );      
    }
  }
}

