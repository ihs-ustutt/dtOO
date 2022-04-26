#include "bVOAddFace.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOAddFace::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOAddFace, new bVOAddFace())
  );
  
  bVOAddFace::bVOAddFace() {
  }

  bVOAddFace::~bVOAddFace() {
    
  }
  
  void bVOAddFace::bVOAddFace::init( 
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
		//   name="bVOAddFace" 
    //   regionLabel="labelA"
		//   faceLabel="{labelA}{labelB}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStr("regionLabel", element);
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element);    
  }
  
  void bVOAddFace::preUpdate( void ) {
		dt__ptrAss( dtGmshModel * gm, ptrBoundedVolume()->getModel() );
		
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    dtGmshRegion * const gr = gm->getDtGmshRegionByPhysical(_regionLabel);

    dt__forAllRefAuto(_faceLabel, aFace) {
      gr->addFace( gm->getDtGmshFaceByPhysical(aFace), 1 );
    }
  }
}
