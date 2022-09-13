#include "bVORecombine.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshFace.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVORecombine::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVORecombine, new bVORecombine())
  );
  
  bVORecombine::bVORecombine() {
  }

  bVORecombine::~bVORecombine() {
    
  }
  
  void bVORecombine::bVORecombine::init( 
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
    
		// <bVObserver 
		//   name="bVORecombine" 
		//   regionLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_faceLabel
		= 
		dtXmlParserBase::getAttributeStrVector("faceLabel", element);
  }
  
  void bVORecombine::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
		// set current model
		//
		::GModel::setCurrent( gm );
    
    dt__forAllRefAuto(_faceLabel, aLabel) {
      dt__forAllRefAuto( gm->getDtGmshFaceListByPhysical(aLabel), aFace ) {
        aFace->meshRecombine();
      }
    }
  }
}
