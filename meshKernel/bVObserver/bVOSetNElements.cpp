#include "bVOSetNElements.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>

namespace dtOO {  
  bVOSetNElements::bVOSetNElements() {
  }

  bVOSetNElements::~bVOSetNElements() {
    
  }
  
  void bVOSetNElements::bVOSetNElements::init( 
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
		//   name="bVOSetNElements" 
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
    dt__throwIf((_nE.size()!=2) && (_nE.size()!=4), init());    
  }
  
  void bVOSetNElements::preUpdate( void ) {
		dt__ptrAss(
      dtGmshModel * gm, 
      ptrBoundedVolume()->getModel()
    );
		
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
  }
}
