#include "bVOWriteMSH.h"

#include <logMe/logMe.h>
#include <logMe/dtParMacros.h>
#include <xmlHeaven/qtXmlBase.h>
#include <constValueHeaven/constValue.h>
#include <analyticFunctionHeaven/analyticFunction.h>
#include <analyticGeometryHeaven/analyticGeometry.h>
#include <boundedVolume.h>
#include <xmlHeaven/dtXmlParser.h>
#include <meshEngine/dtGmshModel.h>

namespace dtOO {  
  bVOWriteMSH::bVOWriteMSH() {
  }

  bVOWriteMSH::~bVOWriteMSH() {
    
  }
  
  void bVOWriteMSH::bVOWriteMSH::init( 
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
    
//		<bVObserver name="bVOWriteMSH" 
//		  filename="mesh.msh"
//		  saveAll="false"
//		/>									
		_filename 
		= 
		qtXmlBase::getAttributeStr("filename", element);		
    
    _saveAll = false;
    if (qtXmlBase::hasAttribute("saveAll", element)) {
      _saveAll = qtXmlBase::getAttributeBool("saveAll", element);
    }
    _saveParametric = false;
//    if (qtXmlBase::hasAttribute("saveParametric", element)) {
//      _saveParametric = qtXmlBase::getAttributeBool("saveParametric", element);
//    } 
  }
  
  void bVOWriteMSH::postUpdate( void ) {
    dt__onlyMaster {
      if ( !ptrBoundedVolume()->isMeshed() ) {
        dt__info(postUpdate(), << "Not yet meshed.");        
        
        return;      
      }

      //
      // create filename string if empty
      //
      std::string cFileName = _filename;
      if ( cFileName == "" ) {
        cFileName 
        = 
        dtXmlParser::constReference().currentState()
        +
        "_"
        +
        ptrBoundedVolume()->getLabel()
        +
        ".msh";
      }
//      else dt__throwUnexpected(postUpdate);
      
      dt__info(
        postUpdate(),
        << "Write >" << cFileName << "<." << std::endl
        << dt__eval(_saveAll) << std::endl
        << dt__eval(_saveParametric)
      );
      
		  ptrBoundedVolume()->getModel()->writeMSH(
        cFileName, 2.2, false, _saveAll, _saveParametric
      );
    }
  }
}
