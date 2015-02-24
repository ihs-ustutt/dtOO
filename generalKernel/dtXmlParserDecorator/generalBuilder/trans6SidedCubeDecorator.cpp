#include "trans6SidedCubeDecorator.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <functionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/geoBuilder/bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h>
#include <geometryEngine/dtSurface.h>

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
      std::vector< QDomElement > wElementVec 
			= 
			getChildVector( "analyticGeometry", **toBuildP );
			if (wElementVec.size() == 6) {
				int counter = 0;
				dt__FORALLITER(std::vector< QDomElement >, wElementVec, it) {
					//
					// get analyticGeometry
					//
					analyticGeometry * aG = depAGeoP->get( getAttributeStr("label", *it) );

					//
					// check if it is a map2dTo3d
					//
					dt__MUSTDOWNCAST(aG, map2dTo3d const, mm[counter]);
					counter++;
				}
			}
			else if (wElementVec.size() == 2) {
					//
					// get analyticGeometry
					//
					analyticGeometry * aG0 = depAGeoP->get( getAttributeStr("label", wElementVec[0]) );
					analyticGeometry * aG1 = depAGeoP->get( getAttributeStr("label", wElementVec[1]) );

					//
					// check if it is a map2dTo3d
					//
					dt__PTRASS(analyticSurface const * aS0, analyticSurface::ConstDownCast(aG0));
					dt__PTRASS(analyticSurface const * aS1, analyticSurface::ConstDownCast(aG1));
					
					vectorHandling< dtSurface const * > cDtS;
					vectorHandling< dtSurface * > dtS;
					cDtS.push_back( aS0->ptrDtSurface() );
					cDtS.push_back( aS1->ptrDtSurface() );
					dtS = bSplineSurfaces_bSplineSurfaceSkinConstructOCC(cDtS).result();
					dt__FORALLINDEX(dtS, ii) {
						mm[ii] = new analyticSurface(dtS[ii]);
					}
					
					dtS.destroy();
			}			
			else {
				dt__THROW(
					buildPart(), 
					<< DTLOGEVAL(wElementVec.size()) << LOGDEL
					<< "Supported only 2 and 6." << LOGDEL
				);
			}

      //
      // create analyticGeometry
      //      
      aGeoP->push_back( new trans6SidedCube(mm[0], mm[1], mm[2], mm[3], mm[4], mm[5]) );
    }
  }
}
