#include "bVONameRegions.h"

#include <map3dTo3dBlockGmsh.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <dtGmshRegionHex.h>
#include <dtGmshModel.h>

namespace dtOO {  
  bVONameRegions::bVONameRegions() {
  }

  bVONameRegions::~bVONameRegions() {
    
  }
  
  void bVONameRegions::bVONameRegions::init( 
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
		//   name="bVONameRegions" 
		//   faceLabel="{name0}"
		// />
								
    DTINFOWF(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStringVector("regionLabel", element);
  }
  
  void bVONameRegions::update( void ) {
		dtGmshModel * gm = ptrBoundedVolume()->getModel();
		
		dt__THROW_IF(gm==NULL, update());
		
		int counter = 0;
		for(GModel::riter r_it = gm->firstRegion(); r_it != gm->lastRegion(); ++r_it) {
			std::vector< int > pInt = (*r_it)->getPhysicalEntities();
			dt__THROW_IF(pInt.size()!=0, update());

			std::string newL = _regionLabel[counter];
			int pTag = (*r_it)->model()->setPhysicalName(newL, 3, 0);
			(*r_it)->addPhysicalEntity(pTag);
			DTINFOWF(
				update(),
				<< logMe::dtFormat("Name physical group %s (%d) and add region %d of %s") 
					% pTag % newL % counter % ptrBoundedVolume()->getLabel()
			);						
			counter++;
		}
  }
}
