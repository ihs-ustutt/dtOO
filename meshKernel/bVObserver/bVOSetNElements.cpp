#include "bVOSetNElements.h"

#include <map3dTo3dCompoundVolume.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <dtXmlParserDecorator/dtXmlParserBase.h>
#include <dtGmshRegionHex.h>
#include <dtGmshModel.h>

namespace dtOO {  
  bVOSetNElements::bVOSetNElements() {
  }

  bVOSetNElements::~bVOSetNElements() {
    
  }
  
  void bVOSetNElements::bVOSetNElements::init( 
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
		//   name="bVOSetNElements" 
		//   number_elements_u="10" 
		//   number_elements_v="100" 
		//   number_elements_w="10"
		// />
								
    DTINFOWF(init(), << dtXmlParserBase::convertToString(element) );
		_nU 
		= 
		dtXmlParserBase::getAttributeIntMuParse(
			"number_elements_u", element, cV, aF
		);
    _nV 
		= 
		dtXmlParserBase::getAttributeIntMuParse(
			"number_elements_v", element, cV, aF
		);
    _nW 
		= 
		dtXmlParserBase::getAttributeIntMuParse(
			"number_elements_w", element, cV, aF
		);
  }
  
  void bVOSetNElements::update( void ) {
    dt__PTRASS(
			map3dTo3dCompoundVolume * mC,
      map3dTo3dCompoundVolume::DownCast(ptrBoundedVolume())
		);
		
		dtGmshModel * gm = mC->refDtGmshModel();
		
		for(GModel::riter r_it = gm->firstRegion(); r_it != gm->lastRegion(); ++r_it) {
		  dtGmshRegionHex * hex = dtGmshRegionHex::DownCast(*r_it);
			if (hex) hex->meshTransfinite(_nU+1, _nV+1, _nW+1);
		}
  }
}
