#include "bVOSetSimpleGrading.h"

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
  bVOSetSimpleGrading::bVOSetSimpleGrading() {
  }

  bVOSetSimpleGrading::~bVOSetSimpleGrading() {
    
  }
  
  void bVOSetSimpleGrading::bVOSetSimpleGrading::init( 
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
		//   name="bVOSetSimpleGrading" 
		//   grading="{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}"
		// />
								
    DTINFOWF(init(), << dtXmlParserBase::convertToString(element) );
		_grading
		= 
		dtXmlParserBase::getAttributeFloatVectorMuParse("grading", element, cV, aF);
		_type
		= 
		dtXmlParserBase::getAttributeFloatVectorMuParse("type", element, cV, aF);
//		for ( auto &el : _grading ) DTINFOWF(init(), << el);
  }
  
  void bVOSetSimpleGrading::preUpdate( void ) {
		dtGmshModel * gm = ptrBoundedVolume()->getModel();
		
		dt__THROW_IF(gm==NULL, preUpdate());
		
		for(::GModel::riter r_it = gm->firstRegion(); r_it != gm->lastRegion(); ++r_it) {
		  dtGmshRegionHex * hex = dtGmshRegionHex::DownCast(*r_it);

			dt__WARN_IF(hex==NULL, preUpdate());

			if (hex) hex->setGrading(
			           _grading[0], _grading[1], _grading[2], 
							   _type[0], _type[1], _type[2]
			         );
		}
  }
}
