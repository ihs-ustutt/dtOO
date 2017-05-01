#include "bVOSetGradingToFaceRule.h"

#include <logMe/logMe.h>
#include <logMe/dtMacros.h>
#include <interfaceHeaven/ptrHandling.h>
#include <vector>
#include <math.h>
#include <progHelper.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParserBase.h>
#include <meshEngine/dtGmshEdge.h>
#include <meshEngine/dtGmshFace.h>
#include <meshEngine/dtGmshRegion.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOSetGradingToFaceRule::bVOSetGradingToFaceRule() {
  }

  bVOSetGradingToFaceRule::~bVOSetGradingToFaceRule() {
    
  }
  
  void bVOSetGradingToFaceRule::bVOSetGradingToFaceRule::init( 
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
    
//    <bVObserver 
//      name="bVOSetGradingToFaceRule"
//  		rule="
//        {:aS_skinCorpus_dtIn*::3::-1:}
//			"    
//     />
								
    dt__info(init(), << dtXmlParserBase::convertToString(element) );
		_rule
		= 
		dtXmlParserBase::getAttributeStrVector("rule", element);
  }
  
  void bVOSetGradingToFaceRule::preUpdate( void ) {
		dt__ptrAss(dtGmshModel * gm, ptrBoundedVolume()->getModel());
    
		//
		// set current model
		//
		::GModel::setCurrent( gm );

    logContainer< bVOSetGradingToFaceRule > logC(logINFO, "preUpdate");    
    dt__forAllRefAuto( _rule, aRule ) {
      //
      // create components
      //
      std::vector< std::string > comp
      = 
      dtXmlParserBase::convertToStringVector(":", ":", aRule);
      dt__throwIf(comp.size()!=4, preUpdate());
      
      //
      // assign components
      //
      std::string const & fStr0 = comp[0];
      std::string const & fStr1 = comp[1];
      float const & fType = stringPrimitive::stringToFloat( comp[2] );
      float const & fGrading = stringPrimitive::stringToFloat( comp[3] );
    
      //
      // output
      //
      logC() 
        << aRule << std::endl
        << ">" << fStr0 << "< -> >" << fStr1 << "< : " 
        << fType << " / " << fGrading 
        << std::endl;

      
      //
      // loop all edges
      //
      dt__forAllRefAuto(gm->dtEdges(), anEdge) {
        bool breakEdge = false;        
        //
        // fStr0 / beginVertex -> endVertex / fStr1
        //
        dt__forAllRefAuto(anEdge->getBeginVertex()->faces(), aFaceStart) {
          if (breakEdge) break;
          if (
            dtXmlParserBase::matchWildcard( 
              fStr0, gm->getPhysicalString(aFaceStart) 
            )
          ) {
            dt__forAllRefAuto(anEdge->getEndVertex()->faces(), aFaceEnd) {
              if (
                dtXmlParserBase::matchWildcard( 
                  fStr1, gm->getPhysicalString(aFaceEnd) 
                )
              ) {
                logC() 
                  << "  edge[ tag = " << anEdge->tag() << " ] : ( " 
                  << gm->getPhysicalString(aFaceStart) << " ) -> ( " 
                  << gm->getPhysicalString(aFaceEnd) << " )" << std::endl;
                anEdge->setGrading(fGrading, fType);
                breakEdge = true;
                break;
              }
            }
          }
        }
        
        //
        // fStr1 / beginVertex -> endVertex / fStr0
        //
        dt__forAllRefAuto(anEdge->getBeginVertex()->faces(), aFaceStart) {
          if (breakEdge) break;
          if (
            dtXmlParserBase::matchWildcard( 
              fStr1, gm->getPhysicalString(aFaceStart) 
            )
          ) {
            dt__forAllRefAuto(anEdge->getEndVertex()->faces(), aFaceEnd) {
              if (
                dtXmlParserBase::matchWildcard( 
                  fStr0, gm->getPhysicalString(aFaceEnd) 
                )
              ) {
            
                logC() 
                  << "  reverse edge[ tag = " << anEdge->tag() << " ] : ( " 
                  << gm->getPhysicalString(aFaceStart) << " ) -> ( " 
                  << gm->getPhysicalString(aFaceEnd) << " )" << std::endl;
                anEdge->setGrading(-fGrading, fType);
                breakEdge = true;
                break;                
              }
            }
          }
        }
      }
    }        
  }
}
