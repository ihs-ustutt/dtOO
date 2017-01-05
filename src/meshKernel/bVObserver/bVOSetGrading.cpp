#include "bVOSetGrading.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOSetGrading::bVOSetGrading() {
  }

  bVOSetGrading::~bVOSetGrading() {
    
  }
  
  void bVOSetGrading::bVOSetGrading::init( 
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
		//   name="bVOSetGrading"
    //   regionLabel="{REG1}{REG2}"
		//   grading="{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}"
		//   type="{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}"    
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);    
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element); 
    
    dt__throwIf(_regionLabel.size() && _faceLabel.size(), init());
		
    _grading
		= 
		dtXmlParserBase::getAttributeFloatVectorMuParse("grading", element, cV, aF);
		_type
		= 
		dtXmlParserBase::getAttributeFloatVectorMuParse("type", element, cV, aF);
  }
  
  void bVOSetGrading::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
	
    dt__forAllConstIter(std::vector< std::string >, _regionLabel, rIt) {
      gm->getDtGmshRegionByPhysical(*rIt)->setGrading(_grading, _type);
    }    
    dt__forAllConstIter(std::vector< std::string >, _faceLabel, fIt) {
      gm->getDtGmshFaceByPhysical(*fIt)->setGrading(_grading, _type);
    }        
  }
}
