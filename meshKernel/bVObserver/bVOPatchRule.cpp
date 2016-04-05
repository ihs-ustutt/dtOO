#include "bVOPatchRule.h"

#include <logMe/logMe.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOPatchRule::bVOPatchRule() {
  }

  bVOPatchRule::~bVOPatchRule() {
    
  }
  
  void bVOPatchRule::bVOPatchRule::init( 
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
    
//		<bVObserver name="bVOPatchRule" 
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
  
  void bVOPatchRule::postUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
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
        if ( qtXmlBase::stringContains("*", oldPhys) ) {
          std::string wildCardString 
          = 
          qtXmlBase::stringRemoveSingle("*", oldPhys);
          //
          // pattern with wildcard (*)
          //
          if ( qtXmlBase::stringContains(wildCardString, currentPhysical) ) {
            dt__info(
              postUpdate(), 
              << "Handling wildcard string :> " 
              << oldPhys << " = " << wildCardString << std::endl
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
        if ( qtXmlBase::stringContains("*", oldPhys) ) {
          std::string wildCardString 
          = 
          qtXmlBase::stringRemoveSingle("*", oldPhys);
          //
          // pattern with wildcard (*)
          //
          if ( qtXmlBase::stringContains(wildCardString, currentPhysical) ) {
            dt__info(
              postUpdate(), 
              << "Handling wildcard string :> " 
              << oldPhys << " = " << wildCardString << std::endl
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
