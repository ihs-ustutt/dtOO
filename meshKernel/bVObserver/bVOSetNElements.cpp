#include "bVOSetNElements.h"

#include <map3dTo3dBlockGmsh.h>
#include <logMe/logMe.h>
#include <progHelper.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshRegionHex.h>
#include <meshEngine/dtGmshModel.h>

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
		//   number_elements="{10}{100}{10}" 
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		
		std::vector< int > nn 
		= 
		dtXmlParserBase::getAttributeIntVectorMuParse(
			"number_elements", element, cV, aF
		);
		_nU = nn[0];
		_nV = nn[1];
		_nW = nn[2];
  }
  
  void bVOSetNElements::preUpdate( void ) {
		dtGmshModel * gm = ptrBoundedVolume()->getModel();
		
		dt__throwIf(gm==NULL, preUpdate());
		
		for(::GModel::riter r_it = gm->firstRegion(); r_it != gm->lastRegion(); ++r_it) {
		  dtGmshRegionHex * hex = dtGmshRegionHex::DownCast(*r_it);
			if (hex) hex->meshTransfinite(_nU+1, _nV+1, _nW+1);
		}
  }
}
