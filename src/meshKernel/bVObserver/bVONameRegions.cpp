#include "bVONameRegions.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <logMe/logContainer.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshModel.h>
#include "bVOInterfaceFactory.h"

namespace dtOO {
  bool bVONameRegions::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVONameRegions, new bVONameRegions())
  );
  
  bVONameRegions::bVONameRegions() {
  }

  bVONameRegions::~bVONameRegions() {
    
  }
  
  void bVONameRegions::bVONameRegions::init( 
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
		//   name="bVONameRegions" 
		//   faceLabel="{name0}"
		// />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_regionLabel
		= 
		dtXmlParserBase::getAttributeStrVector("regionLabel", element);
  }
  
  void bVONameRegions::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );
    
		//
		// check size
		//		
		dt__throwIf(
      !_regionLabel.empty()&&(_regionLabel.size()!=gm->getNumRegions()), 
      preUpdate()
    );
		
    logContainer< bVONameRegions > logC(logINFO, "preUpdate()");
    if ( _regionLabel.empty() ) {
      logC() << "Apply automatic naming" << std::endl;
      dtInt cc = 0;
      dt__forAllRefAuto(gm->regions(), aReg) {
        if ( gm->getPhysicalString(aReg) == "" ) {
          gm->tagPhysical( aReg, "R_"+stringPrimitive::intToString(cc) );
        }
        logC() << "Region : " << gm->getPhysicalString(aReg) << std::endl;
        cc++;
      }      
    }
    else {
      dtInt counter = 0;
      dt__forAllRefAuto(gm->regions(), aReg) {
        std::vector< dtInt > pInt = aReg->getPhysicalEntities();
        dt__throwIf(pInt.size()!=0, preUpdate());

        std::string newL = _regionLabel[counter];

        if (newL != "") {
          dtInt pTag = aReg->model()->setPhysicalName(newL, 3, 0);
          aReg->addPhysicalEntity(pTag);
          dtGmshModel::intGEntityVMap map;
          gm->getPhysicalGroups(3, map);
          logC()
            << logMe::dtFormat(
              "Physical group %d / %s ( %d regions ) -> add region %d"
            )
            % newL % pTag % map[ pTag ].size() % aReg->tag() 
            << std::endl;											        
        }
        counter++;		      
      }
    }
  }
}
