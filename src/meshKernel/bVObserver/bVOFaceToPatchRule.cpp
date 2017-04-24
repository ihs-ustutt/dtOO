#include "bVOFaceToPatchRule.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>

#include <boost/assign.hpp>
namespace dtOO {  
  bVOFaceToPatchRule::bVOFaceToPatchRule() {
  }

  bVOFaceToPatchRule::~bVOFaceToPatchRule() {
    
  }
  
  std::vector< std::string > bVOFaceToPatchRule::factoryAlias( void ) const {
    return ::boost::assign::list_of("bVOPatchRule");
  }
  
  void bVOFaceToPatchRule::bVOFaceToPatchRule::init( 
		::QDomElement const & element,
		baseContainer const * const bC,
		cVPtrVec const * const cV,
		aFPtrVec const * const aF,
		aGPtrVec const * const aG,
		bVPtrVec const * const bV,
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

    _patchRule = qtXmlBase::getAttributeStrVector("patchRule", element);
    _regRule = qtXmlBase::getAttributeStrVector("regRule", element);
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

    std::vector< std::string > gf_toDelPhysical; 
    dt__forAllConstIter(std::vector< std::string >, _patchRule, it) {
      std::string newPhys = *it;
      std::string oldPhys 
      = 
      qtXmlBase::getStringBetweenAndRemove(":", ":", &newPhys);
      newPhys = qtXmlBase::getStringBetween(":", ":", newPhys);

      dt__forAllConstIter(dtGmshModel::intGEntityVMap, gf_number, nIt) {
        std::string currentPhysical = gm->getPhysicalName(2, nIt->first);
        bool addThis = false;
        if ( qtXmlBase::isWildcard(oldPhys) ) {
          //
          // pattern with wildcard (*)
          //
          if ( qtXmlBase::matchWildcard(oldPhys, currentPhysical) ) {
            dt__info(
              postUpdate(), 
              << "Handling wildcard string :> " << oldPhys << std::endl
              << "Adding :> " << currentPhysical << std::endl
              << "To     :> " << newPhys
            );
            //
            // do not delete existing patches
            //
            if (currentPhysical != newPhys ) {
              gf_toDelPhysical.push_back( currentPhysical );
            }
            addThis = true;
          }
        }
        else {
          //
          // physical matches exactly given pattern
          //
          if ( oldPhys == currentPhysical ) {
            dt__info(
              postUpdate(), 
              << "Handling normal string :> " << oldPhys << std::endl
              << "Adding :> " << currentPhysical << std::endl
              << "To     :> " << newPhys
            );

            addThis = true;           
          }
        }

        //
        // add patches
        //
        if (addThis) {
          dt__forAllIndex(nIt->second, ii) {
            gm->untagPhysical( nIt->second[ii] );
            if (!newPhys.empty()) {
              gm->tagPhysical( nIt->second[ii], newPhys );
            }
          }            
        }
      }
    }
    dt__info(postUpdate(), << "Delete :> " << gf_toDelPhysical);      
    dt__forAllIndex(gf_toDelPhysical, ii) {
      gm->deletePhysicalGroup(
        2, 
        gm->getPhysicalNumber(2, gf_toDelPhysical[ii])
      );
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

    std::vector< std::string > gr_toDelPhysical; 
    dt__forAllConstIter(std::vector< std::string >, _regRule, it) {
      std::string newPhys = *it;
      std::string oldPhys 
      = 
      qtXmlBase::getStringBetweenAndRemove(":", ":", &newPhys);
      newPhys = qtXmlBase::getStringBetween(":", ":", newPhys);

      dt__forAllConstIter(dtGmshModel::intGEntityVMap, gr_number, nIt) {
        std::string currentPhysical = gm->getPhysicalName(3, nIt->first);
        bool addThis = false;
        if ( qtXmlBase::isWildcard(oldPhys) ) {
          //
          // pattern with wildcard (*)
          //
          if ( qtXmlBase::matchWildcard(oldPhys, currentPhysical) ) {
            dt__info(
              postUpdate(), 
              << "Handling wildcard string :> " << oldPhys << std::endl
              << "Adding :> " << currentPhysical << std::endl
              << "To     :> " << newPhys
            );
            //
            // do not delete existing patches
            //
            if (currentPhysical != newPhys ) {
              gr_toDelPhysical.push_back( currentPhysical );
            }
            addThis = true;
          }
        }
        else {
          //
          // physical matches exactly given pattern
          //
          if ( oldPhys == currentPhysical ) {
            dt__info(
              postUpdate(), 
              << "Handling normal string :> " << oldPhys << std::endl
              << "Adding :> " << currentPhysical << std::endl
              << "To     :> " << newPhys
            );

            addThis = true;
          }
        }

        //
        // add patches
        //
        if (addThis) {
          dt__forAllIndex(nIt->second, ii) {
            if (!newPhys.empty()) {
              gm->tagPhysical( nIt->second[ii], newPhys );
            }
          }            
        }
      }
    }
    dt__info(postUpdate(), << "Delete :> " << gr_toDelPhysical);      
    dt__forAllIndex(gr_toDelPhysical, ii) {
      gm->deletePhysicalGroup(
        3, 
        gm->getPhysicalNumber(3, gr_toDelPhysical[ii])
      );
    }
    
    //
    // remove empty physical groups
    //
    gm->removeEmptyPhysicals();
  }
}
