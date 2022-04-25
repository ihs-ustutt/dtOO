#include "bVORemoveRegions.h"

#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshVertex.h>

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVORemoveRegions::bVORemoveRegions() {
  }

  bVORemoveRegions::~bVORemoveRegions() {
    
  }
  
  void bVORemoveRegions::bVORemoveRegions::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
		boundedVolume * attachTo
  ) {
    //
    // init bVOInterface
    //
    bVOInterface::init(element, bC, cV, aF, aG, bV, attachTo);
    
		// <bVObserver 
		//   name="bVORemoveRegions" 
		//   regionLabel="{name0}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);
  }
  
  void bVORemoveRegions::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
    	
    logContainer< bVORemoveRegions > logC(logINFO, "preUpdate()");
    dt__forAllRefAuto(_regionLabel, aLabel) {
      std::list< dtGmshRegion * > regL 
      = 
      gm->getDtGmshRegionListByPhysical( aLabel );
      dt__forAllRefAuto(regL, aReg) {
        //
        // remove region on faces
        //
        dt__forAllRefAuto( aReg->dtFaces(), aFace ) aFace->delRegion( aReg );
        //
        // remove region in model
        //
        gm->remove( aReg );
      } 
    }
    //
    // remove all faces that do not belong to at least one region
    //
    dt__forAllRefAuto( gm->dtFaces(), aFace ) {
      if ( aFace->dtRegions().empty() ) {
        //
        // delte face from all edges
        //
        dt__forAllRefAuto( aFace->dtEdges(), aEdge ) aEdge->delFace( aFace );
        gm->remove( aFace );
      }
    }
    //
    // remove all edges that do not belong to at least one face
    //
    dt__forAllRefAuto( gm->dtEdges(), aEdge ) {
      if ( aEdge->dtFaces().empty() ) {
        //
        // delte face from all edges
        //
        dt__forAllRefAuto( aEdge->dtVertices(), aVertex ) aVertex->delEdge( aEdge );
        gm->remove( aEdge );
      }
    }    
    //
    // remove all vertices that do not belong to at least one edge
    //
    dt__forAllRefAuto( gm->dtVertices(), aVertex ) {
      if ( aVertex->dtEdges().empty() ) {
        //
        // delte face from all edges
        //
        gm->remove( aVertex );
      }
    }        
  }
}
