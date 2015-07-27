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
//		/>									

    _patchRule = qtXmlBase::getAttributeStrVector("patchRule", element);
  }
  
  void bVOPatchRule::postUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
		
    dtGmshModel::intGEntityVMap ge_number;
    gm->getPhysicalGroups(2, ge_number);      

    std::vector< std::string > toDelPhysical; 
    dt__forAllConstIter(std::vector< std::string >, _patchRule, it) {
      std::string newPhys = *it;
      std::string oldPhys 
      = 
      qtXmlBase::getStringBetweenAndRemove(":", ":", &newPhys);
      newPhys = qtXmlBase::getStringBetween(":", ":", newPhys);

      dt__forAllConstIter(dtGmshModel::intGEntityVMap, ge_number, nIt) {
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
              toDelPhysical.push_back( currentPhysical );
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
    dt__info(postUpdate(), << "Delete :> " << toDelPhysical);      
    dt__forAllIndex(toDelPhysical, ii) {
      gm->deletePhysicalGroup(
        2, 
        gm->getPhysicalNumber(2, toDelPhysical[ii])
      );
    }
  }
}
