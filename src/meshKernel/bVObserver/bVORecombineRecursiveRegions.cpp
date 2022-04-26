#include "bVORecombineRecursiveRegions.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVORecombineRecursiveRegions::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(
      bVORecombineRecursiveRegions, new bVORecombineRecursiveRegions()
    )
  );
  
  bVORecombineRecursiveRegions::bVORecombineRecursiveRegions() {
  }

  bVORecombineRecursiveRegions::~bVORecombineRecursiveRegions() {
    
  }
  
  void bVORecombineRecursiveRegions::bVORecombineRecursiveRegions::init( 
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
		//   name="bVORecombineRecursiveRegions" 
		//   regionLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);
  }
  
  void bVORecombineRecursiveRegions::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    dt__forAllConstIter(std::vector< std::string >, _regionLabel, rLIt) {
      gm->getDtGmshRegionByPhysical(*rLIt)->meshRecombineRecursive();
    }
  }
}
