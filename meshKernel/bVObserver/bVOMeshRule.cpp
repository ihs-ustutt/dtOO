#include "bVOMeshRule.h"

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
  bVOMeshRule::bVOMeshRule() {
  }

  bVOMeshRule::~bVOMeshRule() {
    
  }
  
  void bVOMeshRule::bVOMeshRule::init( 
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
		//   name="bVOMeshRule" 
		//   grading="{1.}{1.}{1.}"
		//   type="{1.}{1.}{1.}"    
		// />
								
  }
  
  void bVOMeshRule::preUpdate( void ) {
		dtGmshModel * gm = ptrBoundedVolume()->getModel();
		
		dt__throwIf(gm==NULL, preUpdate());
  }
}
