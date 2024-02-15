/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >
    
    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "bVOFaceToPatchRule.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshFace.h>

#include <boost/assign.hpp>
#include "bVOInterfaceFactory.h"

namespace dtOO {  
  bool bVOFaceToPatchRule::_registrated 
  =
  bVOInterfaceFactory::registrate(
    dt__tmpPtr(bVOFaceToPatchRule, new bVOFaceToPatchRule())
  );
  
  bVOFaceToPatchRule::bVOFaceToPatchRule() {
  }

  bVOFaceToPatchRule::~bVOFaceToPatchRule() {
    
  }
  
  std::vector< std::string > bVOFaceToPatchRule::factoryAlias( void ) const {
    return ::boost::assign::list_of("bVOPatchRule");
  }
  
  void bVOFaceToPatchRule::init( 
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
    
//		<bVObserver name="bVOFaceToPatchRule" 
//  		patchRule="
//    		{:HUB*::HUB:}
//		  	{:LAIN::LAIN:}
//		  "
//  		regRule="
//    		{:REG*::REG:}
//		  "    
//		/>									
    jsonPrimitive jE;
    jE.append< std::vector< std::string > >(
      "_patchRule", 
      qtXmlBase::getAttributeStrVector("patchRule", element)
     );
    jE.append< std::vector< std::string > >(
      "_regRule",
      qtXmlBase::getAttributeStrVector("regRule", element)
    );
    bVOInterface::jInit(jE, bC, cV, aF, aG, bV, attachTo);
  }
  
  void bVOFaceToPatchRule::postUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    //
    // set current model
    //
    ::GModel::setCurrent(gm);
    
    //
    // 2D
    //
    dtGmshModel::intGEntityVMap gf_number;
    gm->getPhysicalGroups(2, gf_number);      

    dt__forAllRefAuto(
      config().lookup< std::vector< std::string > >("_patchRule"), aRule
    ) {
      std::string newPhys = aRule;
      std::string oldPhys 
      = 
      qtXmlBase::getStringBetweenAndRemove(":", ":", &newPhys);
      newPhys = qtXmlBase::getStringBetween(":", ":", newPhys);

      dt__forAllRefAuto(gm->dtFaces(), aFace) {
        if ( gm->matchWildCardPhysical( oldPhys, aFace ) ) {
          dt__info(
            postUpdate(), 
            << "Handling rule :> " << oldPhys << std::endl
            << "Adding :> " << aFace->getPhysicalString() << std::endl
            << "To     :> " << newPhys
          );
          gm->untagPhysical( aFace );
          if ( !newPhys.empty() ) gm->tagPhysical( aFace, newPhys );
        }
      }
    }
    
    
    //
    // remove empty physical groups
    //
    gm->removeEmptyPhysicals();
    
    //
    // 3D
    //
    dtGmshModel::intGEntityVMap gr_number;
    gm->getPhysicalGroups(3, gr_number);      

    dt__forAllRefAuto(
      config().lookup< std::vector< std::string > >("_regRule"), aRule
    ) {
      std::string newPhys = aRule;
      std::string oldPhys 
      = 
      qtXmlBase::getStringBetweenAndRemove(":", ":", &newPhys);
      newPhys = qtXmlBase::getStringBetween(":", ":", newPhys);

      dt__forAllRefAuto(gm->dtRegions(), aRegion) {
        if ( gm->matchWildCardPhysical( oldPhys, aRegion ) ) {
          dt__info(
            postUpdate(), 
            << "Handling rule :> " << oldPhys << std::endl
            << "Adding :> " << aRegion->getPhysicalString() << std::endl
            << "To     :> " << newPhys
          );
          gm->untagPhysical( aRegion );
          if ( !newPhys.empty() ) gm->tagPhysical( aRegion, newPhys );
        }
      }
    }
    
    //
    // remove empty physical groups
    //
    gm->removeEmptyPhysicals();
  }
}
