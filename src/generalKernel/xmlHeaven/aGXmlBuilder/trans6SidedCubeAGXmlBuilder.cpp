#include "trans6SidedCubeAGXmlBuilder.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
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
    ::QDomElement const & toBuild,
    baseContainer * const bC,
    cVPtrVec const * const cV,  
    aFPtrVec const * const aF,  
    aGPtrVec const * const aG,
    aGPtrVec * result
  ) const {
    //
		// check input
		//    
    dt__throwIf(
      !dtXmlParserBase::hasChild("analyticGeometry", toBuild), 
      buildPart()
    );
    
    dt__pVH(map2dTo3d) mm;
    std::vector< ::QDomElement > wElementVec 
    = 
    dtXmlParserBase::getChildVector( "analyticGeometry", toBuild );
    if (wElementVec.size() == 6) {
      int counter = 0;
      dt__forAllIter(std::vector< ::QDomElement >, wElementVec, it) {
        //
        // get analyticGeometry
        //
        dt__pH(analyticGeometry const) aG_t(
          dtXmlParserBase::createAnalyticGeometry(*it, bC, cV, aF, aG)
        );          
        //
        // check if it is a map2dTo3d
        //
        mm.push_back( map2dTo3d::MustDownCast(aG_t->clone()) );
        counter++;
      }
    }
    else if (wElementVec.size() == 2) {
      //
      // get analyticGeometry
      //
      dt__pH(analyticGeometry const) aG0_t(
        dtXmlParserBase::createAnalyticGeometry(&wElementVec[0], bC, cV, aF, aG)
      );      
      dt__pH(analyticGeometry const) aG1_t(
        dtXmlParserBase::createAnalyticGeometry(&wElementVec[1], bC, cV, aF, aG)
      );            
      //
      // check if it is a map2dTo3d
      //
      dt__ptrAss(
        analyticSurface const * aS0, analyticSurface::ConstDownCast(aG0_t.get())
      );
      dt__ptrAss(
        analyticSurface const * aS1, analyticSurface::ConstDownCast(aG1_t.get())
      );

      vectorHandling< dtSurface const * > cDtS;
      vectorHandling< dtSurface * > dtS;
      cDtS.push_back( aS0->ptrDtSurface() );
      cDtS.push_back( aS1->ptrDtSurface() );
      dtS = bSplineSurfaces_bSplineSurfaceSkinConstructOCC(cDtS).result();
      dt__forAllIndex(dtS, ii) {
        mm.push_back( new analyticSurface(dtS[ii]) );
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
      new trans6SidedCube(
        &(mm[0]), &(mm[1]), &(mm[2]), &(mm[3]), &(mm[4]), &(mm[5])
      ) 
    );
  }
}
