#include "bVOSetNElements.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>

namespace dtOO {  
  bVOSetNElements::bVOSetNElements() {
  }

  bVOSetNElements::~bVOSetNElements() {
    
  }
  
  void bVOSetNElements::bVOSetNElements::init( 
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
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector(
      "faceLabel", element, std::vector< std::string >()
    );
    
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector(
      "regionLabel", element, std::vector< std::string >()
    );    
		_nE
		= 
		dtXmlParserBase::getAttributeIntVectorMuParse(
			"numberElements", element, cV, aF
		);
  }
  
  void bVOSetNElements::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
		
    dt__forAllConstIter(std::vector< std::string >, _faceLabel, fLIt) {
      if (_nE.size() == 2) {
        gm->getDtGmshFaceByPhysical(*fLIt)->meshWNElements(
          _nE[0], _nE[1]
        );
      }
      else {
        gm->getDtGmshFaceByPhysical(*fLIt)->meshWNElements(
          _nE[0], _nE[1], _nE[2], _nE[3]
        );        
      }
    }
    dt__forAllConstIter(std::vector< std::string >, _regionLabel, rLIt) {
      if (_nE.size() == 3) {
        gm->getDtGmshRegionByPhysical(*rLIt)->meshWNElements(
          _nE[0], _nE[1], _nE[2]
        );
      }
    }    
  }
}
