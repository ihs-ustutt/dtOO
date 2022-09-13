#include "bVOTransfiniteRegions.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOTransfiniteRegions::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOTransfiniteRegions, new bVOTransfiniteRegions())
  );
  
  bVOTransfiniteRegions::bVOTransfiniteRegions() {
  }

  bVOTransfiniteRegions::~bVOTransfiniteRegions() {
    
  }
  
  void bVOTransfiniteRegions::bVOTransfiniteRegions::init( 
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
		//   name="bVOTransfiniteRegions" 
		//   regionLabel="{name0}{name1}{name2}{name3}{name4}{name5}"
		// />
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
    
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);
		_nE
		= 
		dtXmlParserBase::getAttributeIntVectorMuParse(
			"numberElements", element, cV, aF, aG
		);
    dt__throwIf(_nE.size()!=3, init());      
  }
  
  void bVOTransfiniteRegions::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
		
    dt__forAllRefAuto(_regionLabel, aLabel) {
      gm->getDtGmshRegionByPhysical(aLabel)->meshTransfiniteRecursive();
      gm->getDtGmshRegionByPhysical(aLabel)->meshWNElements(
        _nE[0], _nE[1], _nE[2]
      );
    }
		dt__forAllRefAuto(gm->faces(), aFace) {
			dtGmshModel::cast2DtGmshFace(aFace)->correctIfTransfinite();
		}	        
  }
}
