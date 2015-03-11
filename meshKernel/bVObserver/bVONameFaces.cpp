#include "bVONameFaces.h"

#include <map3dTo3dBlockGmsh.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <meshEngine/dtGmshRegionHex.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVONameFaces::bVONameFaces() {
  }

  bVONameFaces::~bVONameFaces() {
    
  }
  
  void bVONameFaces::bVONameFaces::init( 
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
		//   name="bVONameFaces" 
		//   faceLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
								
    DTINFOWF(init(), << dtXmlParserBase::convertToString(element) );
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element);
  }
  
  void bVONameFaces::update( void ) {
		dtGmshModel * gm = ptrBoundedVolume()->getModel();
		
		dt__THROW_IF(gm==NULL, update());
		
//		for(::GModel::riter r_it = gm->firstRegion(); r_it != gm->lastRegion(); ++r_it) {
			std::list< ::GFace * > faceL = gm->faces();
			std::list< ::GFace * >::iterator f_it;

			//
			// check size
			//
			dt__THROW_IFWM(
				_faceLabel.size()!=faceL.size(), update(),
				<< DTLOGEVAL(_faceLabel.size()) << LOGDEL
        << DTLOGEVAL(faceL.size()) 
			);
			
			int counter = 0;
			for (f_it = faceL.begin(); f_it!=faceL.end(); ++f_it) {
				std::vector< int > pInt = (*f_it)->getPhysicalEntities();
				dt__THROW_IF(pInt.size()!=0, update());

				std::string newL = _faceLabel[counter];
				if (newL != "") {
					int pTag = (*f_it)->model()->setPhysicalName(newL, 2, 0);
					(*f_it)->addPhysicalEntity(pTag);
					DTINFOWF(
						update(),
						<< logMe::dtFormat("Name physical group %s (%d) and add face %d of %s") 
							% pTag % newL % counter % ptrBoundedVolume()->getLabel()
					);						
				}
				counter++;				
			}
//		}
  }
}
