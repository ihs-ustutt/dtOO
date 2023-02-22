#include "bVOAddInternalEdge.h"
#include "analyticGeometryHeaven/map1dTo3d.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <analyticGeometryHeaven/map1dTo3d.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>

#include <gmsh/GEdgeLoop.h>
#include <progHelper.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOAddInternalEdge::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOAddInternalEdge, new bVOAddInternalEdge())
  );
  
  bVOAddInternalEdge::bVOAddInternalEdge() {
  }

  bVOAddInternalEdge::~bVOAddInternalEdge() {
    
  }
  
  void bVOAddInternalEdge::bVOAddInternalEdge::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
		boundedVolume * attachTo
  ) {
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
        
		// <bVObserver 
		//   name="bVOAddInternalEdge" 
		//   regionLabel="name0"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
    jsonPrimitive jE;
		jE.append< std::string >(
      "_regionLabel",
      dtXmlParserBase::getAttributeStr("regionLabel", element)
    );
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOAddInternalEdge::preUpdate( void ) {
		dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
		
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    dtGmshRegion * const gr 
    = 
    gm->getDtGmshRegionByPhysical(
      config().lookup< std::string >("_regionLabel")
    );
    
    std::vector< dtGmshFace * > fV = progHelper::list2Vector( gr->dtFaces() );
    
    std::vector< GEdge * > e0;
    std::vector< GEdge * > e2;
    dt__forFromToIndex(6, fV.size(), ii) {
      std::vector< ::GEdge * > eV = fV[ii]->edges();
      e0.push_back( eV[0] );
      e2.push_back( eV[2] );
    }
    
    ::GEdgeLoop el0(e0);
    for(::GEdgeLoop::citer it = el0.begin(); it != el0.end(); ++it){
      fV[0]->addEdge( it->getEdge(), it->getSign() );
    }
    ::GEdgeLoop el2(e2);
    for(::GEdgeLoop::citer it = el2.begin(); it != el2.end(); ++it){
      fV[1]->addEdge( it->getEdge(), it->getSign() );
    }
  }
}
