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

namespace dtOO {  
  bVOAddInternalEdge::bVOAddInternalEdge() {
  }

  bVOAddInternalEdge::~bVOAddInternalEdge() {
    
  }
  
  void bVOAddInternalEdge::bVOAddInternalEdge::init( 
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
		//   name="bVOAddInternalEdge" 
		//   faceLabel="name0"
		//   edgeAnalyticGeometry="{labelA}{labelB}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStr("faceLabel", element);

    dt__forAllRefAuto(
      dtXmlParserBase::getAttributeStrVector("edgeAnalyticGeometry", element), 
      aLabel
    ) {
      dt__ptrAss( 
        map1dTo3d const * const m1d, 
        map1dTo3d::ConstDownCast( aG->get(aLabel) )
      );
      _internalEdge.push_back( m1d );
    }
  }
  
  void bVOAddInternalEdge::preUpdate( void ) {
		dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
		
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    dtGmshFace * const gf = gm->getDtGmshFaceByPhysical(_faceLabel);
    
    dt__forAllRefAuto(_internalEdge, anEdge) {
      int edgeTag;
      gm->addIfEdgeToGmshModel( anEdge, &edgeTag );
      
      dt__info( 
        preUpdate(),
        << "Adding internal edge = " 
        << edgeTag << " to faceLabel = " << _faceLabel
      );
      
      gf->addEdge( gm->getEdgeByTag(edgeTag), 1);
    }
  }
}
