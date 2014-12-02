#include "trans6SidedCubeDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  trans6SidedCubeDecorator::trans6SidedCubeDecorator() {
  }

  trans6SidedCubeDecorator::~trans6SidedCubeDecorator() {
  }

  void trans6SidedCubeDecorator::buildPart( 
    QDomElement ** toBuildP,
    baseContainer * const bC,     
    vectorHandling< constValue * > const * const cValP,           
    vectorHandling< analyticFunction * > const * const sFunP,    
    vectorHandling< analyticGeometry * > const * const depAGeoP,
    vectorHandling< analyticGeometry * > * aGeoP 
  ) const {
    bool hasAGeo = hasChild("analyticGeometry", **toBuildP);
    
    if ( hasAGeo ) {
      std::vector< map2dTo3d const * > mm(6, NULL);
      QDomElement wElement = getChild( "analyticGeometry", **toBuildP );
      for (int ii=0; ii<6; ii++) {
        //
        // get analyticGeometry
        //
        analyticGeometry * aG = depAGeoP->get( getAttributeStr("label", wElement) );
        
        //
        // check if it is a map2dTo3d
        //
        dt__MUSTDOWNCAST(aG, map2dTo3d const, mm[ii]);
        //
        // next sibling
        //
        wElement = getNextSibling("analyticGeometry", wElement);
      }

      //
      // create analyticGeometry
      //      
      aGeoP->push_back( new trans6SidedCube(mm[0], mm[1], mm[2], mm[3], mm[4], mm[5]) );
    }
  }
}
