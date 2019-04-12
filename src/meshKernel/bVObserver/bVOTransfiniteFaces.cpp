#include "bVOTransfiniteFaces.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>

namespace dtOO {  
  bVOTransfiniteFaces::bVOTransfiniteFaces() {
  }

  bVOTransfiniteFaces::~bVOTransfiniteFaces() {
    
  }
  
  void bVOTransfiniteFaces::bVOTransfiniteFaces::init( 
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
		//   name="bVOTransfiniteFaces" 
		//   faceLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		//   numberElements="{numU}{numV}"    
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element);
		_nE
		= 
		dtXmlParserBase::getAttributeIntVectorMuParse(
			"numberElements", element, cV, aF
		);
    dt__throwIf(_nE.size()!=2, init());    
  }
  
  void bVOTransfiniteFaces::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
		
    dt__forAllRefAuto(_faceLabel, aLabel) {
      dt__forAllRefAuto( gm->getDtGmshFaceListByPhysical(aLabel), aFace ) {
        aFace->meshTransfinite();
        aFace->meshWNElements( _nE[0], _nE[1] );
      }
    }
		std::list< ::GFace * > ff = gm->faces();
		dt__forAllIter(std::list< ::GFace * >, ff, it) {
			dtGmshModel::cast2DtGmshFace(*it)->correctIfTransfinite();
		}	    
  }
}
