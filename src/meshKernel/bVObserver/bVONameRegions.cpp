#include "bVONameRegions.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVONameRegions::bVONameRegions() {
  }

  bVONameRegions::~bVONameRegions() {
    
  }
  
  void bVONameRegions::bVONameRegions::init( 
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
		//   name="bVONameRegions" 
		//   faceLabel="{name0}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);
  }
  
  void bVONameRegions::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
		//
		// check size
		//		
		dt__throwIf(_regionLabel.size()!=gm->getNumRegions(), preUpdate());
		
		int counter = 0;
    logContainer< bVONameRegions > logC(logINFO, "preUpdate()");
		for ( 
      ::GModel::riter r_it = gm->firstRegion(); 
      r_it != gm->lastRegion(); 
      ++r_it
    ) {
			std::vector< int > pInt = (*r_it)->getPhysicalEntities();
			dt__throwIf(pInt.size()!=0, preUpdate());

			std::string newL = _regionLabel[counter];
			
			if (newL != "") {
				int pTag = (*r_it)->model()->setPhysicalName(newL, 3, 0);
				(*r_it)->addPhysicalEntity(pTag);
        dtGmshModel::intGEntityVMap map;
        gm->getPhysicalGroups(3, map);
				logC()
					<< logMe::dtFormat(
            "Physical group %d / %s ( %d regions ) -> add region %d"
          )
					% newL % pTag % map[ pTag ].size() % (*r_it)->tag() 
          << std::endl;											        
			}
			counter++;		      
		}
  }
}