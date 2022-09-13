#include "bVOSetNElements.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOSetNElements::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOSetNElements, new bVOSetNElements())
  );
  
  bVOSetNElements::bVOSetNElements() {
  }

  bVOSetNElements::~bVOSetNElements() {
    
  }
  
  void bVOSetNElements::bVOSetNElements::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		lvH_constValue const * const cV,
		lvH_analyticFunction const * const aF,
		lvH_analyticGeometry const * const aG,
		lvH_boundedVolume const * const bV,
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
		
    dt__forAllRefAuto(_faceLabel, aLabel) {
      if (_nE.size() == 2) {
        dt__forAllRefAuto(gm->getDtGmshFaceListByPhysical(aLabel), aFace) {
          aFace->meshWNElements(_nE[0], _nE[1]);
        }
      }
      else {
        dt__forAllRefAuto(gm->getDtGmshFaceListByPhysical(aLabel), aFace) {        
          aFace->meshWNElements( _nE[0], _nE[1], _nE[2], _nE[3] );        
        }
      }
    }
    dt__forAllRefAuto(_regionLabel, aLabel) {
      if (_nE.size() == 3) {
        dt__forAllRefAuto(gm->getDtGmshRegionListByPhysical(aLabel), aReg) {        
          aReg->meshWNElements( _nE[0], _nE[1], _nE[2] );
        }
      }
    }    
  }
}
