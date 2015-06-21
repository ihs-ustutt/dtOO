#include "bVOTransfiniteRegions.h"

#include <logMe/logMe.h>
#include <progHelper.h>
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
		QDomElement const & element,
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
		//   name="bVOTransfiniteRegions" 
		//   regionLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);
  }
  
  void bVOTransfiniteRegions::preUpdate( void ) {
		dtGmshModel * gm = ptrBoundedVolume()->getModel();
		dt__throwIf(gm==NULL, update());
		
    dt__forAllConstIter(std::vector< std::string >, _regionLabel, rLIt) {
      gm->getDtGmshRegionByPhysical(*rLIt)->meshTransfinite();
    }
  }
}
