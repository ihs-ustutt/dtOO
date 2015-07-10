#include "bVOSetGrading.h"

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
  bVOSetGrading::bVOSetGrading() {
  }

  bVOSetGrading::~bVOSetGrading() {
    
  }
  
  void bVOSetGrading::bVOSetGrading::init( 
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
		//   name="bVOSetGrading"
    //   regionLabel="{REG1}{REG2}"
		//   grading="{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}"
		//   type="{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}{1.}"    
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);    
		_grading
		= 
		dtXmlParserBase::getAttributeFloatVectorMuParse("grading", element, cV, aF);
		_type
		= 
		dtXmlParserBase::getAttributeFloatVectorMuParse("type", element, cV, aF);
//		for ( auto &el : _grading ) dt__info(init(), << el);
  }
  
  void bVOSetGrading::preUpdate( void ) {
		dtGmshModel * gm = ptrBoundedVolume()->getModel();
		
		dt__throwIf(gm==NULL, preUpdate());
	
    dt__forAllConstIter(std::vector< std::string >, _regionLabel, rIt) {
      gm->getDtGmshRegionByPhysical(*rIt)->setGrading(_grading, _type);
    }    
  }
}
