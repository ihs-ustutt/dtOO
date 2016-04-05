#include "bVOAddFace.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>

namespace dtOO {  
  bVOAddFace::bVOAddFace() {
  }

  bVOAddFace::~bVOAddFace() {
    
  }
  
  void bVOAddFace::bVOAddFace::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		vectorHandling< constValue * > const * const cV,
		vectorHandling< analyticFunction * > const * const aF,
		vectorHandling< analyticGeometry * > const * const aG,
		vectorHandling< boundedVolume * > const * const bV,
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
		
    dtGmshRegion * const gr = gm->getDtGmshRegionByPhysical(_regionLabel);

    dt__forAllRefAuto(_faceLabel, aFace) {
      gr->addFace( gm->getDtGmshFaceByPhysical(aFace), 1 );
    }
  }
}
