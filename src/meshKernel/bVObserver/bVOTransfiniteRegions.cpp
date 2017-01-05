#include "bVOTransfiniteRegions.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>

namespace dtOO {  
  bVOTransfiniteRegions::bVOTransfiniteRegions() {
  }

  bVOTransfiniteRegions::~bVOTransfiniteRegions() {
    
  }
  
  void bVOTransfiniteRegions::bVOTransfiniteRegions::init( 
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
		//   name="bVOTransfiniteRegions" 
		//   regionLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);
  }
  
  void bVOTransfiniteRegions::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
		
    dt__forAllConstIter(std::vector< std::string >, _regionLabel, rLIt) {
      gm->getDtGmshRegionByPhysical(*rLIt)->meshTransfinite();
    }
  }
}
