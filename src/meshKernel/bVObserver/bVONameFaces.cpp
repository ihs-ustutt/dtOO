#include "bVONameFaces.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVONameFaces::bVONameFaces() {
  }

  bVONameFaces::~bVONameFaces() {
    
  }
  
  void bVONameFaces::bVONameFaces::init( 
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
		//   name="bVONameFaces" 
		//   faceLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element);
  }
  
  void bVONameFaces::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
		//
		// set current model
		//
		::GModel::setCurrent( gm );
    
    std::list< ::GFace * > faceL = gm->faces();
    std::list< ::GFace * >::iterator f_it;

    //
    // check size
    //
    dt__throwIfWithMessage(
      _faceLabel.size()!=faceL.size(), preUpdate(),
      << dt__eval(_faceLabel.size()) << std::endl
      << dt__eval(faceL.size()) 
    );

    int counter = 0;
    logContainer< bVONameFaces > logC(logINFO, "preUpdate()");      
    for (f_it = faceL.begin(); f_it!=faceL.end(); ++f_it) {
      std::vector< int > pInt = (*f_it)->getPhysicalEntities();
      dt__throwIf(pInt.size()!=0, preUpdate());

      std::string newL = _faceLabel[counter];
      if (newL != "") {
        int pTag = (*f_it)->model()->setPhysicalName(newL, 2, 0);
        (*f_it)->addPhysicalEntity(pTag);
        dtGmshModel::intGEntityVMap map;
        gm->getPhysicalGroups(2, map);          
        logC()
          << logMe::dtFormat(
            "Physical group %d / %s ( %d faces ) -> add face %d"
          ) 
          % newL % pTag % map[ pTag ].size() % (*f_it)->tag() 
          << std::endl;
      }
      counter++;				
    }
  }
}
