#include "trans6SidedCubeAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <dtLinearAlgebra.h>
#include <analyticGeometryHeaven/trans6SidedCube.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/analyticSurface.h>
#include <analyticGeometryHeaven/map2dTo3d.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <constValueHeaven/constValue.h>
#include <baseContainerHeaven/baseContainer.h>
#include <geometryEngine/geoBuilder/bSplineSurfaces_bSplineSurfaceSkinConstructOCC.h>
#include <geometryEngine/dtSurface.h>

#include <QtXml/QDomElement>
#include <QtXml/QDomNode>

namespace dtOO {
  trans6SidedCubeAGXmlBuilder::trans6SidedCubeAGXmlBuilder() {
  }

  trans6SidedCubeAGXmlBuilder::~trans6SidedCubeAGXmlBuilder() {
  }

  void trans6SidedCubeAGXmlBuilder::buildPart( 
    QDomElement const & toBuild,
    baseContainer * const bC,
    vectorHandling< constValue * > const * const cV,  
    vectorHandling< analyticFunction * > const * const aF,  
    vectorHandling< analyticGeometry * > const * const aG,
    vectorHandling< analyticGeometry * > * result
  ) const {
    //
		// check input
		//    
    dt__throwIf(!dtXmlParserBase::hasChild("analyticGeometry", toBuild), buildPart());
    
    std::vector< map2dTo3d const * > mm(6, NULL);
    std::vector< QDomElement > wElementVec 
    = 
    dtXmlParserBase::getChildVector( "analyticGeometry", toBuild );
    if (wElementVec.size() == 6) {
      int counter = 0;
      dt__forAllIter(std::vector< QDomElement >, wElementVec, it) {
        //
        // get analyticGeometry
        //
        analyticGeometry const * aG_t = aG->get( dtXmlParserBase::getAttributeStr("label", *it) );

        //
        // check if it is a map2dTo3d
        //
        dt__ptrAss(mm[counter], map2dTo3d::ConstDownCast(aG_t));
        counter++;
      }
    }
    else if (wElementVec.size() == 2) {
      //
      // get analyticGeometry
      //
      analyticGeometry const * aG0_t 
      = 
      aG->get( dtXmlParserBase::getAttributeStr("label", wElementVec[0]) );
      analyticGeometry const * aG1_t 
      = 
      aG->get( dtXmlParserBase::getAttributeStr("label", wElementVec[1]) );

      //
      // check if it is a map2dTo3d
      //
      dt__ptrAss(
        analyticSurface const * aS0, analyticSurface::ConstDownCast(aG0_t)
      );
      dt__ptrAss(
        analyticSurface const * aS1, analyticSurface::ConstDownCast(aG1_t)
      );

      vectorHandling< dtSurface const * > cDtS;
      vectorHandling< dtSurface * > dtS;
      cDtS.push_back( aS0->ptrDtSurface() );
      cDtS.push_back( aS1->ptrDtSurface() );
      dtS = bSplineSurfaces_bSplineSurfaceSkinConstructOCC(cDtS).result();
      dt__forAllIndex(dtS, ii) {
        mm[ii] = new analyticSurface(dtS[ii]);
      }

      dtS.destroy();
    }			
    else {
      dt__throw(
        buildPart(), 
        << dt__eval(wElementVec.size()) << std::endl
        << "Supported only 2 and 6." << std::endl
      );
    }

    //
    // create analyticGeometry
    //      
    result->push_back( 
      new trans6SidedCube(mm[0], mm[1], mm[2], mm[3], mm[4], mm[5]) 
    );
  }
}
